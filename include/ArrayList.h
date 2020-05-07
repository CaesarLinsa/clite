#ifndef ARRAYLIST_H
#define ARRAYLIST_H

typedef struct  ArrayList ArrayList;

struct ArrayList {
    void** data;
    unsigned int length;
    unsigned int capacity;
    void (*append)(ArrayList* list,  void* element);
    int (*remove)(ArrayList* list, void* element);
    int (*is_element_exsits)(ArrayList* list, void* element);
    void* (*get)(ArrayList* list,int index)
};

void init_list(ArrayList* list, int inital_capacity);
void append(ArrayList* list,  void* element);
int remove_obj(ArrayList* list, void* element);
int is_element_exsits(ArrayList* list, void* element);
void* get_obj(ArrayList* list,int index);
#endif