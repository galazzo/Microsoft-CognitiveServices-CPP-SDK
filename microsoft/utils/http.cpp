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

static size_t OnReceiveData(void * pData, size_t tSize, size_t tCount, void* userp)
{
    size_t length = tSize * tCount, index = 0;
    while (index < length)
    {
        unsigned char *temp = (unsigned char *)pData + index;
        if ((temp[0] == '\r') || (temp[0] == '\n'))
            break;
        index++;
    }

    std::string str((unsigned char*)pData, (unsigned char*)pData + index);		
    
	struct HttpResponse *wt = (struct HttpResponse *)userp;	
	
	//std::map<std::string, std::string>* pmHeader = (std::map<std::string, std::string>*)pmUser;
	wt->raw_headers.append(str); wt->raw_headers.append("\n");
	
    size_t pos = str.find(": ");
    if (pos != std::string::npos)
        wt->headers.insert(std::pair<std::string, std::string> (str.substr(0, pos), str.substr(pos + 2)));

    return (tCount);
}


HttpResponse post(std::string url, std::string fields, std::map<std::string, std::string>* headers, HttpContent* body)
{
    CURL *curl = NULL;
    CURLcode res;

    HttpResponse result;
	
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
		result.status = -1;
		return result;
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
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result.content);

            // get verbose debug output please
            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			
			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, OnReceiveData);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, &result);
			 
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
                result.status = -1;
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            } else {
				result.status = CURLE_OK;
				cout << result.raw_headers;
			}

            // always cleanup
            curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return result;
}
 
