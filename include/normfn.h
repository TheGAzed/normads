#ifndef NORMFN_H
#define NORMFN_H

#include <stddef.h>
#include <stdbool.h>

/// @brief Function pointer to destroy a single element in data structure. Based on 'free';
typedef void   (*destroy_fn) (void * element);
/// @brief Function pointer to copy a single element in data structure. Based on 'memcpy' and 'memmove'.
typedef void * (*copy_fn) (void * dest, const void * src, size_t size);
/// @brief Function pointer to compare two elements, 'zero' if equal, 'positive' if first is bigger, and 'negative' if second.
/// Based on 'memcmp'.
typedef int    (*compare_fn) (const void * a, const void * b, size_t size);
/// @brief Fucntion pointer to perform a single operation on element in data structure.
typedef bool   (*operate_fn) (void * element, size_t size, void * args);
/// @brief Function pointer to manage an array of finite number of element in data structure.
typedef void   (*manage_fn) (void * base, size_t n, size_t size, void * arg);

#endif // NORMFN_H
