
#ifdef __cplusplus
extern "C" {
#endif
typedef struct ArrayList {
    void** data;
    unsigned int length;
    unsigned int capacity;
} list;

void list_init(size_t inital_capacity, ArrayList* list);
void list_append(ArrayList* list, void* element);
#ifdef __cplusplus
};
#endif



