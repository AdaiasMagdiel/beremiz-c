#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_array.h"

int array_init(Array *a, size_t element_size) {
    a->array = malloc(element_size * INITIAL_ARRAY_CAPACITY);

    if (a->array == NULL) {
        return -1;
    }

    a->length = 0;
    a->size = INITIAL_ARRAY_CAPACITY;
    a->element_size = element_size;

    return 0;
}

int array_push(Array *a, void *element) {
    if (a->length == a->size) {
        a->size += INITIAL_ARRAY_CAPACITY;
        void *new_array = realloc(a->array, a->size * a->element_size);

        if (new_array == NULL) {
            return -1;
        }

        a->array = new_array;
    }

    memcpy((char*)a->array + (a->length * a->element_size), element, a->element_size);
    a->length++;

    return 0;
}

int array_pop(Array *a, void *element) {
    if (a->length == 0) {
        return -1;
    }
    a->length--;

    memcpy(element, (char*)a->array + (a->length * a->element_size), a->element_size);

    if (a->length > 0 && a->length <= a->size / 4) {
        a->size /= 2;
        void *new_array = realloc(a->array, a->size * a->element_size);

        if (new_array == NULL) {
            return -1;
        }

        a->array = new_array;
    }

    return 0;
}

int array_remove(Array *a, int index, void *element) {
    if (index < 0 || index >= a->length) {
        return -1; 
    }
    
    memcpy(element, (char*)a->array + (index * a->element_size), a->element_size);
    
    if (index < a->length - 1) {
        memmove((char*)a->array + (index * a->element_size), 
                (char*)a->array + ((index + 1) * a->element_size), 
                (a->length - index - 1) * a->element_size);
    }
    a->length--;

    
    if (a->length > 0 && a->length <= a->size / 4) {
        a->size /= 2;
        void *new_array = realloc(a->array, a->size * a->element_size);

        if (new_array == NULL) {
            return -1; 
        }

        a->array = new_array;
    }

    return 0; 
}

int array_insert(Array *a, int index, void *element) {
    if (index < 0 || index > a->length) {
        return -1;
    }
   
    if (a->length == a->size) {
        a->size *= 2;
        void *new_array = realloc(a->array, a->size * a->element_size);

        if (new_array == NULL) {
            return -1;
        }

        a->array = new_array;
    }
   
    if (index < a->length) {
        memmove((char*)a->array + ((index + 1) * a->element_size), 
                (char*)a->array + (index * a->element_size), 
                (a->length - index) * a->element_size);
    }

    memcpy((char*)a->array + (index * a->element_size), element, a->element_size);
    a->length++;
    return 0;
}

void array_free(Array *a) {
    free(a->array);
    a->array = NULL;
    a->length = 0;
    a->size = 0;
    a->element_size = 0;
}
