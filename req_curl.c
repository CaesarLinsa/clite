#include <gumbo.h>
#include "stdio.h"
#include "stdlib.h"
#include "curl/curl.h"
#include "include/req_curl.h"
#include "file_operate.h"
#include "string.h"


static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,void *userp)
{
    size_t	realsize =	size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    mem->memory	= (char*) realloc(mem->memory, mem->size + realsize	+ 1);
    if(mem->memory	==	NULL)
    {
        /*	out	of	memory!	*/
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), contents,	realsize);
    printf("contents:%s", contents);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

static size_t write_file(void *buffer, size_t size, size_t nmemb,FILE *fp)
{
    size_t written = fwrite(buffer, size, nmemb, fp);
    return written;
}

char* req_get(char* url)
{
    CURL * curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = (char*) malloc(1);
    chunk.size = 0;
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("caught an error");
        }
    }
    curl_easy_cleanup(curl);
    return chunk.memory;
}

void download_file(char* dir, char* url)
{
    CURL * curl;
    CURLcode res;
    char* filename=file_name(url);
    strcat(dir,filename);
    FILE *fp = fopen(dir, "wb");
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLE_OPERATION_TIMEOUTED, 120);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("caught an error");
        }
    }
    curl_easy_cleanup(curl);
    fclose(fp);
}
