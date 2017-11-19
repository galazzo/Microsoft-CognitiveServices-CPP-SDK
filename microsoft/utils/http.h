 
#ifndef HTTP_UTILS
#define HTTP_UTILS

#include <string>
#include <vector>
#include <map>

using namespace std;

struct HttpContent {
  const char *buffer;
  size_t size;
};

static size_t ReadCallback(void *content, size_t size, size_t nmemb, void *userp);
static size_t WriteCallback(void *content, size_t size, size_t nmemb, void *userp);

std::string post(std::string url, std::string fields, std::map<std::string, std::string>* headers=NULL, HttpContent* body = NULL);

#endif
