#include <gumbo.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include  "cjson/cJSON.h"
#include "file_operate.h"
#include "include/ArrayList.h"
#include "include/req_curl.h"
#include  "include/tag_operate.h"

#define  CATCH_USL "www.my1231.com"
#define  PATH  "/home/caesar/cirros/"
#define  CATCH_USL_MM "www.my1231.com/mm"

char* get_main_url(int i)
{
    if(i == 1)
        return CATCH_USL;
    else {
        char* st = malloc(strlen(CATCH_USL) + strlen("/mm/list_1_html")+4);
        sprintf(st, "%s%s%d%s", CATCH_USL, "/mm/list_1_", i, ".html");
        return st;
    }
}
int is_string_contain_charactor(char* str, char* charactor)
{
    char* dest = malloc(strlen(str) + 1);
    strcpy(dest,str);
    char* index = strtok(dest, charactor);
    if(strcmp(str,index) == 0){
        free(dest);
        return 0;
    }
    free(dest);
    return 1;
}

void curl_data(char* a_href,char* dir)
{
    char* a_href_url= (char*) malloc(strlen(a_href) +1 +strlen(CATCH_USL_MM));
    char* li_a_href = string_trim(a_href);
    if(!is_string_contain_charactor(li_a_href, "_"))
    {
        sprintf(a_href_url, "%s/%s", CATCH_USL, li_a_href);
    }else {
        sprintf(a_href_url, "%s/%s",CATCH_USL_MM , li_a_href);
    }
    printf("path:%s\n", a_href_url);
    char* data = req_get(a_href_url);
    GumboOutput* output = gumbo_parse(data);
    free(data);
    GumboNode* root = output->root;
    cJSON* json = get_div_a_img_tags(root);
    gumbo_destroy_output(&kGumboDefaultOptions,output);
    char* href = string_trim(cJSON_Print(cJSON_GetObjectItem(json, "href")));
    if(href[0] == '/')
        return;
    char* src = string_trim(cJSON_Print(cJSON_GetObjectItem(json, "src")));
    char* dest = malloc(strlen(src)*2+1);
    strcpy(dest, dir);
    strcat(dest,"/");
    download_file(dest, src);
    free(dest);
    curl_data(href, dir);
}


int main() {
    int  j;
    for (j=9; j<=60;j++) {
        char* main_url = get_main_url(j);
        char *data = req_get(main_url);
        GumboOutput *output = gumbo_parse(data);
        free(data);
        free(main_url);
        GumboNode *root = output->root;
        ArrayList list;
        init_list(&list, 10);
        get_a_img_tags(root, &list);
        gumbo_destroy_output(&kGumboDefaultOptions, output);
        for (int i = 0; i < list.length; i++) {
            char *a_alt = cJSON_Print(cJSON_GetObjectItem(list.data[i], "a_alt"));
            char *str = (char *) malloc(strlen(a_alt) * 2 + 1 + strlen(PATH));
            char *li_a_alt = string_trim(a_alt);
            sprintf(str, "%s%s", PATH, li_a_alt);
            create_dir(str);
            char *a_href = cJSON_Print(cJSON_GetObjectItem(list.data[i], "a_href"));
            curl_data(a_href, str);
            free(str);
        }
        destory_array(&list);

    }
    return 0;
}