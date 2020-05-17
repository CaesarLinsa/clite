#include <gumbo.h>
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include  "cjson/cJSON.h"
#include "include/ArrayList.h"

GumboNode* get_node_by_node_field(GumboNode* root, GumboTag tag, char* field, char* value)
{

    if(root->v.element.tag == tag)
    {
        GumboAttribute* attr = gumbo_get_attribute(&root->v.element.attributes, field);
        if(attr !=NULL && strcmp(attr->value, value) == 0)
            return root;
    }
    GumboVector cnodes = root->v.element.children;
    for(int i=0; i<cnodes.length; i++){
        GumboNode* cnode = cnodes.data[i];
        if(cnode->v.element.tag == tag)
        {
            GumboAttribute* cattr = gumbo_get_attribute(&cnode->v.element.attributes, field);
            if (cattr != NULL && strcmp(cattr->value, value) == 0)
                return cnode;
        }
    }
    return NULL;
}

GumboNode* get_node_by_node_tag(GumboNode* root, GumboTag tag)
{
    if(root->type != GUMBO_NODE_ELEMENT) return NULL;
    if(root->v.element.tag == tag) return root;
    GumboVector cnodes = root->v.element.children;
    for(int i=0; i<cnodes.length; i++){
        GumboNode* cnode = cnodes.data[i];
        if(cnode->v.element.tag == tag) return cnode;
    }
    return NULL;
}


void get_a_img_tags(GumboNode* node, ArrayList* list)
{
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
                        GumboAttribute* a_alt=gumbo_get_attribute(&gnode->v.element.attributes,
                                                                  "alt");
                        // a_alt value contains chinese charactor, which is 2 bytes,wherase
                        //char is 1 byte,so multiply 2
                        int length = strlen("{\"a_href\":,\"a_alt\":}") + strlen(href->value) + strlen(a_alt->value)*2+1;
                        char* img_json = (char*) malloc(length);
                        sprintf(img_json, "{\"a_href\":\"%s\",\"a_alt\":\"%s\"}", href->value,a_alt->value);
                        list->append(list, cJSON_Parse(img_json));
                        free(img_json);
                    }
                }
            }else{
                get_a_img_tags(cnode, list);
            }
        }
    }
    GumboVector* children = &node->v.element.children;
    for(int i=0;i<children->length;i++) {
        GumboNode *cnode = (GumboNode*) children->data[i];
        get_a_img_tags(cnode, list);
    }
}

char* string_trim(char* str)
{
    int i = 0;
    size_t j = strlen(str) - 1;
    char* out = (char*) malloc(strlen(str)+1);
    while(str[i] == '"' || str[i] == ' ')
        i++;
    while(str[j] == '"' || str[i] == ' ')
        j--;
    strncpy(out, str+i, j-i+1);
    out[j-i+1] = '\0';
    return out;
}

cJSON* get_div_a_img_tags(GumboNode* node)
{
    GumboNode* cnode = get_node_by_node_tag(node, GUMBO_TAG_BODY);
    GumboNode* gnode = get_node_by_node_field(cnode, GUMBO_TAG_DIV,"class", "main");
    GumboNode* fnode = get_node_by_node_field(gnode, GUMBO_TAG_DIV, "class", "article");
    GumboNode* tnode = get_node_by_node_field(fnode, GUMBO_TAG_DIV, "class", "content");
    GumboNode* anode =get_node_by_node_tag(tnode, GUMBO_TAG_A);
    GumboAttribute* href=gumbo_get_attribute(&anode->v.element.attributes, "href");
    GumboNode* imgnode =get_node_by_node_tag(anode, GUMBO_TAG_IMG);
    GumboAttribute* src=gumbo_get_attribute(&imgnode->v.element.attributes, "src");
    GumboAttribute* alt=gumbo_get_attribute(&imgnode->v.element.attributes, "alt");
    size_t length = strlen("{\"href\":,\"alt\":,\"src\":}") + strlen(href->value) + strlen(alt->value)*2+ strlen(src)+1;
    char* img_json = (char*) malloc(length * sizeof(char*));
    sprintf(img_json, "{\"href\":\"%s\",\"alt\":\"%s\",\"src\":\"%s\"}", href->value,alt->value, src->value);
    cJSON* json= cJSON_Parse(img_json);
    free(img_json);
    return json;
}