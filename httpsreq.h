#ifndef HTTPSREQ_H
#define HTTPSREQ_H

struct Response {
    char *data;
    size_t size;
};


size_t writeCallback(void *contents, size_t size, size_t nmemb, struct Response *response);

char* makeHttpsRequest(char* url);

void writeToFile(char *filename, char *data);

#endif /* HTTPSREQ_H */

