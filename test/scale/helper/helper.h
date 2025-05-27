#ifndef HELPER_H
#define HELPER_H

#include <stdbool.h>
#include <stddef.h>

#define DATA_TYPE int
#define REALLOC_CHUNK (1 << 5)

typedef struct function_ptr {
    int (*compare)(const void *, const void *);
} function_ptr;

void destroy(void * element);

bool increment(void * element, const size_t size, void * args);

bool print(void * element, const size_t size, void * format);

int compare(const void * a, const void * b);
int compare_reverse(const void * a, const void * b);

void manage(void * elements, const size_t n, const size_t size, void * args);

#endif // HELPER_H
