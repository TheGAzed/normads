#ifndef QUEQNORM_H
#define QUEQNORM_H

#include <normfn.h>

typedef struct quegnorm {
    void * elements; // array of elements
    size_t size, current; // number of elements and index of start element in queue
} quegnorm_s;

/// @brief Creates empty queue.
/// @return Empty queue structure.
quegnorm_s quegnorm_create(void);

/// @brief Destroys a queue.
/// @param queue Queue data structure.
/// @param destroy Function pointer to destroy a single element in queue.
/// @param element_size Size of a single element.
void quegnorm_destroy(quegnorm_s * queue, const destroy_fn destroy, const size_t element_size);

/// @brief Creates a copy of queue and its elements.
/// @param queue Queue data structure.
/// @param copy Function pointer to copy a single element in queue.
/// @param element_size Size of a single element.
/// @return A copy of the queue.
quegnorm_s quegnorm_copy(const quegnorm_s queue, const copy_fn copy, const size_t element_size);

/// @brief Checks if queue size will overflow.
/// @param queue Queue data structure.
/// @return 'true' if queue size will overflow, 'false' otherwise.
bool quegnorm_is_full(const quegnorm_s queue);

/// @brief Checks if queue is empty.
/// @param queue Queue data structure.
/// @return 'true' if queue is empty, 'false' otherwise.
bool quegnorm_is_empty(const quegnorm_s queue);

/// @brief Pushes element to the top of the queue.
/// @param queue Queue data structure.
/// @param element Single element to push.
/// @param element_size Size of a single element.
void quegnorm_push(quegnorm_s * queue, const void * element, const size_t element_size);

/// @brief Peeps the top of the queue.
/// @param queue Queue data structure.
/// @param element Single element to save peeped element into.
/// @param element_size Size of a single element.
void quegnorm_peep(const quegnorm_s queue, void * element, const size_t element_size);

/// @brief Pops element from the top of the queue.
/// @param queue Queue data structure.
/// @param element Single element to save poped element into.
/// @param element_size Size of a single element.
void quegnorm_pop(quegnorm_s * queue, void * element, const size_t element_size);

/// @brief Iterates over each element in queue.
/// @param queue Queue data structure.
/// @param operate Fucntion pointer to perform a single operation on element in queue using arguments.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void quegnorm_foreach(quegnorm_s const * queue, const operate_fn operate, const size_t element_size, void * arguments);

/// @brief Maps each element in queue into an array to manage.
/// @param queue Queue data structure.
/// @param manage Function pointer to manage an array of finite number of element in queue.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void quegnorm_map(quegnorm_s const * queue, const manage_fn manage, const size_t element_size, void * arguments);

#endif //QUEQNORM_H
