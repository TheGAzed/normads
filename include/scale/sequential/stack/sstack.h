#ifndef SSTACK_H
#define SSTACK_H

#include <stddef.h>
#include <stdbool.h>

typedef struct sstack {
    void * elements; // array of elements
    size_t size; // number of elements in stack
} sstack_s;

/// @brief Function pointer to destroy a single element in data structure. Based on 'free';
typedef void   (*destroy_fn) (void * element);
/// @brief Function pointer to copy a single element in data structure. Based on 'memcpy' and 'memmove'.
typedef void * (*copy_fn) (void * dest, const void * src, size_t size);
/// @brief Fucntion pointer to perform a single operation on element in data structure.
typedef bool   (*operate_fn) (void * element, size_t size, void * args);
/// @brief Function pointer to manage an array of finite number of element in data structure.
typedef void   (*manage_fn) (void * base, size_t n, size_t size, void * arg);

/// @brief Creates empty stack.
/// @return Empty stack structure.
sstack_s sstk_create(void);

/// @brief Destroys a stack.
/// @param stack Stack data structure.
/// @param destroy Function pointer to destroy a single element in stack.
/// @param element_size Size of a single element.
void sstk_destroy(sstack_s * stack, const destroy_fn destroy, const size_t element_size);

/// @brief Creates a copy of stack and its elements.
/// @param stack Stack data structure.
/// @param copy Function pointer to copy a single element in stack.
/// @param element_size Size of a single element.
/// @return A copy of the stack.
sstack_s sstk_copy(const sstack_s stack, const copy_fn copy, const size_t element_size);

/// @brief Checks if stack size will overflow.
/// @param stack Stack data structure.
/// @return 'true' if stack size will overflow, 'false' otherwise.
bool sstk_is_full(const sstack_s stack);

/// @brief Checks if stack is empty.
/// @param stack Stack data structure.
/// @return 'true' if stack is empty, 'false' otherwise.
bool sstk_is_empty(const sstack_s stack);

/// @brief Pushes element to the top of the stack.
/// @param stack Stack data structure.
/// @param element Single element to push.
/// @param element_size Size of a single element.
void sstk_push(sstack_s * stack, const void * element, const size_t element_size);

/// @brief Peeps the top of the stack.
/// @param stack Stack data structure.
/// @param element Single element to save peeped element into.
/// @param element_size Size of a single element.
void sstk_peep(const sstack_s stack, void * element, const size_t element_size);

/// @brief Pops element from the top of the stack.
/// @param stack Stack data structure.
/// @param element Single element to save poped element into.
/// @param element_size Size of a single element.
void sstk_pop(sstack_s * stack, void * element, const size_t element_size);

/// @brief Iterates over each element in stack.
/// @param stack Stack data structure.
/// @param operate Fucntion pointer to perform a single operation on element in stack using arguments.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void sstk_foreach(sstack_s const * stack, const operate_fn operate, const size_t element_size, void * arguments);

/// @brief Maps each element in stack into an array to manage.
/// @param stack Stack data structure.
/// @param manage Function pointer to manage an array of finite number of element in stack.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void sstk_map(sstack_s const * stack, const manage_fn manage, const size_t element_size, void * arguments);

#endif // SSTACK_H
