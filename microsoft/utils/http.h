 
#ifndef HTTP_UTILS
#define HTTP_UTILS

#include <string>
#include <vector>
#include <map>
#include <regex>
#include <vector>

using namespace std;

struct HttpContent {
  const char *buffer;
  size_t size;
};

struct HttpResponse {
    int status;
    std::string raw_headers;
    std::map<std::string, std::string> headers;
    std::string content;
};

static size_t ReadCallback(void *content, size_t size, size_t nmemb, void *userp);
static size_t WriteCallback(void *content, size_t size, size_t nmemb, void *userp);

HttpResponse get(std::string url, std::map<std::string, std::string>* headers=NULL);
HttpResponse post(std::string url, std::string fields, std::map<std::string, std::string>* headers=NULL, HttpContent* body = NULL);

bool isUrl(std::string value);

#endif
