#ifndef FETCHER_H
#define FETCHER_H

#include <curl/curl.h>

CURL *fetcher_init(void);
void fetcher_options(CURL *curl, char *url, FILE *file);
int fetcher_perform(CURL *curl);
void fetcher_cleanup(CURL *curl);
size_t
fetcher_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

#endif