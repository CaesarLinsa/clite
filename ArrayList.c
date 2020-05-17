#include "include/ArrayList.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"


void  init_list(ArrayList* list, int inital_capacity)
{
    if(list == NULL)
        list= (ArrayList*) malloc(sizeof(ArrayList));
    list->length = 0;
    list->capacity = inital_capacity;
    if (inital_capacity >0)
    {
        list->data = (void**) malloc(sizeof(void*) * inital_capacity);
    } else
    {
        list->data = NULL;
    }
    list->append = &append;
    list->remove = &remove_obj;
    list->remove = &is_element_exsits;
    list->get = &get_obj;
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

void append(ArrayList* list, void* element)
{
    resize_list_if_full(list);
    assert(list->length< list->capacity);
    list->data[list->length++] = element;
}

int remove_obj(ArrayList* list, void* element)
{
    int i;
    for(i=0;i<list->length;i++)
    {
        if(list->data[i] == element)
            break;
    }
    if (i == list->length-1)
        return -1;
    for(int index=i; index<list->length-1; index++)
        list->data[index] = list->data[index+1];
    list->length -= 1;
    return 0;
}

int is_element_exsits(ArrayList* list, void* element)
{
    for(int i=0;i<list->length; i++)
    {
        if(list->data[i] == element)
            return 0;
    }
    return -1;
}

void* get_obj(ArrayList* list,int index)
{
    if (index < 0 || index >= list->length)
        return NULL;
    return list->data[index];
}

void destory_array(ArrayList* list)
{
    free(list->data);
    free(list);
}
