#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "httpsreq.h"


size_t writeCallback(void *contents, size_t size, size_t nmemb, struct Response *response) {
    size_t totalSize = size * nmemb;
    response->data = realloc(response->data, response->size + totalSize + 1);
    if (response->data == NULL) {
        printf("Hafiza hatasi!\n");
        return 0;
    }
    memcpy(&(response->data[response->size]), contents, totalSize);
    response->size += totalSize;
    response->data[response->size] = '\0';
    return totalSize;
}

char* makeHttpsRequest(char* url) {
    CURL *curl;
    CURLcode res;

    char* result = NULL;

    struct Response response;
    response.data = malloc(1);
    response.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() hata: %s\n", curl_easy_strerror(res));
        } else {
            result = response.data;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return result;
}

void writeToFile(char *filename, char *data) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Dosya acilamadi: %s\n", filename);
        return;
    }
    fprintf(file, "%s", data);
    fclose(file);
}