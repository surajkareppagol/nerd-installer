#include <curl/curl.h>
#include <stdio.h>

#include <fetcher.h>

CURL *fetcher_init(void)
{
  CURL *curl_instance = curl_easy_init();
  return curl_instance;
}

size_t
fetcher_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  size_t write_bytes = fwrite(ptr, size, nmemb, userdata);
  return write_bytes;
}

void fetcher_options(CURL *curl, char *url, FILE *file)
{
  curl_easy_setopt(curl, CURLOPT_URL, url);

  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fetcher_write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
}

int fetcher_perform(CURL *curl)
{
  CURLcode result = curl_easy_perform(curl);

  if (result != CURLE_OK) {
    return 1;
  } else {
    return 0;
  }
}

void fetcher_cleanup(CURL *curl)
{
  curl_easy_cleanup(curl);
}