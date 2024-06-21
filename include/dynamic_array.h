#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#define INITIAL_ARRAY_CAPACITY 8

typedef struct {
    void *array;
    int length;
    int size;
    size_t element_size;
} Array;

int  array_init(Array *a, size_t element_size);
int  array_push(Array *a, void *element);
int  array_pop(Array *a, void *element);
int  array_remove(Array *a, int index, void *element);
int  array_insert(Array *a, int index, void *element);
void array_free(Array *a);

#endif  // DYNAMIC_ARRAY_H
