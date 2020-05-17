#ifndef CAESARLINSA_REQ_CURL_H
#define CAESARLINSA_REQ_CURL_H
typedef  struct MemoryStruct
{
    char *memory;
    size_t size;
};
void download_file(char* dir, char* url);
char* req_get(char* url);
#endif
