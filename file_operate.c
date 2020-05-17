#include "stdlib.h"
#include <sys/stat.h>
#include <unistd.h>
#include "libgen.h"
#include "stdio.h"

int create_dir(char* path)
{
    if(access(path, 0) != 0){
        int ret = mkdir(path, 0755);
        if(ret == -1){
            printf("mkdir failed");
            return -1;
        }
    }
    return 0;
}

char* file_name(char* curl_path)
{
    return basename(curl_path);
}


