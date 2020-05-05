#include "include/util.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"

const ArrayList Empty_Array_List = {NULL, 0, 0};

void  list_init(size_t inital_capacity, ArrayList* list)
{
    list->length = 0;
    list->capacity = inital_capacity;
    if (inital_capacity >0)
    {
    list->data = (void**)malloc(inital_capacity);
    } else
    {
        list->data = NULL;
    }
}


static void resize_list_if_full(ArrayList* list)
{
    if(list->length >= list->capacity)
    {
        if(list->capacity)
        {
            size_t old_num_bytes = sizeof(void*) * list->capacity;
            list->capacity *= 2;
            size_t  num_bytes = sizeof(void*) * list->capacity;
            void** temp =(void**) malloc(num_bytes);
            memcpy(temp, list->data, old_num_bytes);
            free(list->data);
            list->data = temp;
        } else
        {
            list->capacity = 2;
            list->data = (void**) malloc(sizeof(void*) * list->capacity);
        }
    }
}

void list_append(ArrayList* list, void* element)
{
    resize_list_if_full(list);
    assert(list->length< list->capacity);
    list->data[list->length++] = element;
}