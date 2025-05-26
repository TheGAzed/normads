#ifndef STKGNORM_H
#define STKGNORM_H

#include <normfn.h>

typedef struct stkgnorm {
    void * elements; // array of elements
    size_t size; // number of elements in stack
} stkgnorm_s;

/// @brief Creates empty stack.
/// @return Empty stack structure.
stkgnorm_s stkgnorm_create(void);

/// @brief Destroys a stack.
/// @param stack Stack data structure.
/// @param destroy Function pointer to destroy a single element in stack.
/// @param element_size Size of a single element.
void stkgnorm_destroy(stkgnorm_s * stack, const destroy_fn destroy, const size_t element_size);

/// @brief Creates a copy of stack and its elements.
/// @param stack Stack data structure.
/// @param copy Function pointer to copy a single element in stack.
/// @param element_size Size of a single element.
/// @return A copy of the stack.
stkgnorm_s stkgnorm_copy(const stkgnorm_s stack, const copy_fn copy, const size_t element_size);

/// @brief Checks if stack size will overflow.
/// @param stack Stack data structure.
/// @return 'true' if stack size will overflow, 'false' otherwise.
bool stkgnorm_is_full(const stkgnorm_s stack);

/// @brief Checks if stack is empty.
/// @param stack Stack data structure.
/// @return 'true' if stack is empty, 'false' otherwise.
bool stkgnorm_is_empty(const stkgnorm_s stack);

/// @brief Pushes element to the top of the stack.
/// @param stack Stack data structure.
/// @param element Single element to push.
/// @param element_size Size of a single element.
void stkgnorm_push(stkgnorm_s * stack, const void * element, const size_t element_size);

/// @brief Peeps the top of the stack.
/// @param stack Stack data structure.
/// @param element Single element to save peeped element into.
/// @param element_size Size of a single element.
void stkgnorm_peep(const stkgnorm_s stack, void * element, const size_t element_size);

/// @brief Pops element from the top of the stack.
/// @param stack Stack data structure.
/// @param element Single element to save poped element into.
/// @param element_size Size of a single element.
void stkgnorm_pop(stkgnorm_s * stack, void * element, const size_t element_size);

/// @brief Iterates over each element in stack.
/// @param stack Stack data structure.
/// @param operate Fucntion pointer to perform a single operation on element in stack using arguments.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void stkgnorm_foreach(stkgnorm_s const * stack, const operate_fn operate, const size_t element_size, void * arguments);

/// @brief Maps each element in stack into an array to manage.
/// @param stack Stack data structure.
/// @param manage Function pointer to manage an array of finite number of element in stack.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void stkgnorm_map(stkgnorm_s const * stack, const manage_fn manage, const size_t element_size, void * arguments);

#endif // STKGNORM_H
