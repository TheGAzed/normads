#ifndef GNORMSTK_H
#define GNORMSTK_H

#include <normfn.h>

typedef struct gnormstk {
    void * elements; // array of elements
    size_t size; // number of elements in stack
} gnormstk_s;

/// @brief Creates empty stack.
/// @return Empty stack structure.
gnormstk_s gnormstk_create(void);

/// @brief Destroys a stack.
/// @param stack Stack data structure.
/// @param destroy_element Function pointer to destroy a single element in stack.
/// @param element_size Size of a single element.
void gnormstk_destroy(gnormstk_s * stack, const destroy_fn destroy_element, const size_t element_size);

/// @brief Creates a copy of stack and its elements.
/// @param stack Stack data structure.
/// @param copy_element Function pointer to copy a single element in stack.
/// @param element_size Size of a single element.
/// @return A copy of the stack.
gnormstk_s gnormstk_copy(const gnormstk_s stack, const copy_fn copy_element, const size_t element_size);

/// @brief Checks if stack size will overflow.
/// @param stack Stack data structure.
/// @return 'true' if stack size will overflow, 'false' otherwise.
bool gnormstk_is_full(const gnormstk_s stack);

/// @brief Checks if stack is empty.
/// @param stack Stack data structure.
/// @return 'true' if stack is empty, 'false' otherwise.
bool gnormstk_is_empty(const gnormstk_s stack);

/// @brief Pushes element to the top of the stack.
/// @param stack Stack data structure.
/// @param element Single element to push.
/// @param element_size Size of a single element.
void gnormstk_push(gnormstk_s * stack, const void * element, const size_t element_size);

/// @brief Peeps the top of the stack.
/// @param stack Stack data structure.
/// @param element Single element to save peeped element into.
/// @param element_size Size of a single element.
void gnormstk_peep(const gnormstk_s stack, void * element, const size_t element_size);

/// @brief Pops element from the top of the stack.
/// @param stack Stack data structure.
/// @param element Single element to save poped element into.
/// @param element_size Size of a single element.
void gnormstk_pop(gnormstk_s * stack, void * element, const size_t element_size);

/// @brief Iterates over each element in stack.
/// @param stack Stack data structure.
/// @param operate_element Fucntion pointer to perform a single operation on element in stack using arguments.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void gnormstk_foreach(gnormstk_s const * stack, const operate_fn operate_element, const size_t element_size, void * arguments);

/// @brief Maps each element in stack into an array to manage.
/// @param stack Stack data structure.
/// @param manage_element Function pointer to manage an array of finite number of element in stack.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void gnormstk_map(gnormstk_s const * stack, const manage_fn manage_element, const size_t element_size, void * arguments);

#endif // GNORMSTK_H
