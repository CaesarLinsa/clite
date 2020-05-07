#include <gumbo.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include  "cjson/cJSON.h"
#include "include/util.h"

void get_tags(GumboNode* node)
{
    ArrayList list = {NULL, 0, 10};
    list_init(10, &list);
    if(node->type != GUMBO_NODE_ELEMENT) return;
    if(node->v.element.tag == GUMBO_TAG_LI){
        GumboVector* children = &node->v.element.children;
        for(int i=0;i<children->length;i++) {
            GumboNode *cnode = (GumboNode*) children->data[i];
            if(cnode->v.element.tag == GUMBO_TAG_A){
                GumboVector* grantsons = &cnode->v.element.children;
                for(int j=0;j<grantsons->length;j++)
                {
                    GumboNode *gnode = (GumboNode*) grantsons->data[j];
                    if(gnode->v.element.tag == GUMBO_TAG_IMG)
                    {
                        GumboAttribute* href=gumbo_get_attribute(&cnode->v.element.attributes,
                                                                 "href");
                        printf("href:%s\n", href->value);
                        GumboAttribute* a_alt=gumbo_get_attribute(&gnode->v.element.attributes,
                                                                  "alt");
                        printf("a_alt:%s\n", a_alt->value);
                        char* img_json = (char*) malloc(strlen("{a_href:,a_alt:})" + strlen(href->value) +
                        strlen(a_alt->value)));
                        sprintf(img_json, "{a_href:%s,a_alt:%s}", href->value,a_alt->value);
                        printf("%s", img_json);
                        list_append(&list, img_json);
                    }
                }
            }else{
                get_tags(cnode);
            }
        }
    }
    GumboVector* children = &node->v.element.children;
    for(int i=0;i<children->length;i++) {
        GumboNode *cnode = (GumboNode*) children->data[i];
        get_tags(cnode);
    }
}
int main() {
    FILE* f = fopen("/home/caesar/ClionProjects/caesarlinsa/curl.html", "r");
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* data = (char*) malloc(len+1);
    fread(data,len, 1, f);
    fclose(f);
    GumboOutput* output = gumbo_parse(data);
    GumboNode* root = output->root;
    get_tags(root);
    return 0;
}