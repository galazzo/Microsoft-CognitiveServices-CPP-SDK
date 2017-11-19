#include "http.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <curl/curl.h>
 
using namespace std;

static size_t ReadCallback(void *content, size_t size, size_t nmemb, void *userp)
{
	struct HttpContent *wt = (struct HttpContent *)userp;
	size_t buffer_size = size*nmemb;

	if(wt->size) {
		// copy as much as possible from the source to the destination 
		size_t copy_this_much = wt->size;
		if(copy_this_much > buffer_size)
		  copy_this_much = buffer_size;
		memcpy(content, wt->buffer, copy_this_much);

		wt->buffer += copy_this_much;
		wt->size -= copy_this_much;
		return copy_this_much; // we copied this many bytes 
	}

	return 0; // no more data left to deliver 
}

static size_t WriteCallback(void *content, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)content, size * nmemb);
    return size * nmemb;
}

std::string post(std::string url, std::string fields, std::map<std::string, std::string>* headers, HttpContent* body)
{
    CURL *curl = NULL;
    CURLcode res;

    std::string result; // = std::string::empty;
    struct curl_slist *headersList=NULL; // init to NULL is important

    if( headers != NULL)
    {
            std::map<std::string,std::string>::iterator it = headers->begin();
            std::string hvalue;
            for (it=headers->begin(); it!=headers->end(); ++it)
            {
                    hvalue = it->first;
                    hvalue.append(": ");
                    hvalue.append(it->second);

                    //std::cout << hvalue << endl;
                    headersList = curl_slist_append(headersList, hvalue.c_str());
            }
    }

    // In windows, this will init the winsock stuff
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    // Check for errors
    if(res != CURLE_OK) {
            fprintf(stderr, "curl_global_init() failed: %s\n", curl_easy_strerror(res));
            return "-1";
    }

    // get a curl handle
    curl = curl_easy_init();
    if(curl) {

            if( headersList != NULL)
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headersList);

            // First set the URL that is about to receive our POST.
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // Now specify we want to POST data
            curl_easy_setopt(curl, CURLOPT_POST, 1L);

            // we want to use our own read function
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadCallback);

            // pointer to pass to our read function
            curl_easy_setopt(curl, CURLOPT_READDATA, &(*body));

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

            // get verbose debug output please
            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

            /*
            If you use POST to a HTTP 1.1 server, you can send data without knowing
            the size before starting the POST if you use chunked encoding. You
            enable this by adding a header like "Transfer-Encoding: chunked" with
            CURLOPT_HTTPHEADER. With HTTP 1.0 or without chunked transfer, you must
            specify the size in the request.
            */
            #ifdef USE_CHUNKED
            {
                    struct curl_slist *chunk = NULL;

                    chunk = curl_slist_append(chunk, "Transfer-Encoding: chunked");
                    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
                    // use curl_slist_free_all() after the *perform() call to free this list again
            }
            #else
                    // Set the expected POST size. If you want to POST large amounts of data, consider CURLOPT_POSTFIELDSIZE_LARGE
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)body->size);
            #endif

            #ifdef DISABLE_EXPECT
            /*
            Using POST with HTTP 1.1 implies the use of a "Expect: 100-continue"
            header.  You can disable this header with CURLOPT_HTTPHEADER as usual.
            NOTE: if you want chunked transfer too, you need to combine these two
            since you can only set one list of headers with CURLOPT_HTTPHEADER. */

            // A less good option would be to enforce HTTP 1.0, but that might also have other implications.
            {
                    struct curl_slist *chunk = NULL;

                    chunk = curl_slist_append(chunk, "Expect:");
                    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
                    // use curl_slist_free_all() after the *perform() call to free this list again
            }
            #endif

            // Perform the request, res will get the return code
            res = curl_easy_perform(curl);
            if(res != CURLE_OK)
            {
                /*char* ct;
                res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
                if((CURLE_OK == res) && ct != NULL)
                {
                    result.append(ct);
                } else {
                    fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
                }
            } else {*/
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            // always cleanup
            curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return result;
}
 
