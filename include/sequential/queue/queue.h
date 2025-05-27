#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct queue {
    void * elements; // array of elements
    size_t size, current; // number of elements and index of start element in queue
} queue_s;

/// @brief Function pointer to destroy a single element in data structure. Based on 'free';
typedef void   (*destroy_fn) (void * element);
/// @brief Function pointer to copy a single element in data structure. Based on 'memcpy' and 'memmove'.
typedef void * (*copy_fn) (void * dest, const void * src, size_t size);
/// @brief Fucntion pointer to perform a single operation on element in data structure.
typedef bool   (*operate_fn) (void * element, size_t size, void * args);
/// @brief Function pointer to manage an array of finite number of element in data structure.
typedef void   (*manage_fn) (void * base, size_t n, size_t size, void * arg);

/// @brief Creates empty queue.
/// @return Empty queue structure.
queue_s que_create(void);

/// @brief Destroys a queue.
/// @param queue Queue data structure.
/// @param destroy Function pointer to destroy a single element in queue.
/// @param element_size Size of a single element.
void que_destroy(queue_s * queue, const destroy_fn destroy, const size_t element_size);

/// @brief Creates a copy of queue and its elements.
/// @param queue Queue data structure.
/// @param copy Function pointer to copy a single element in queue.
/// @param element_size Size of a single element.
/// @return A copy of the queue.
queue_s que_copy(const queue_s queue, const copy_fn copy, const size_t element_size);

/// @brief Checks if queue size will overflow.
/// @param queue Queue data structure.
/// @return 'true' if queue size will overflow, 'false' otherwise.
bool que_is_full(const queue_s queue);

/// @brief Checks if queue is empty.
/// @param queue Queue data structure.
/// @return 'true' if queue is empty, 'false' otherwise.
bool que_is_empty(const queue_s queue);

/// @brief Enqueues element to the back of the queue.
/// @param queue Queue data structure.
/// @param element Single element to enqueue.
/// @param element_size Size of a single element.
void que_enqueue(queue_s * queue, const void * element, const size_t element_size);

/// @brief Peeks the start of the queue.
/// @param queue Queue data structure.
/// @param element Single element to save peeped element into.
/// @param element_size Size of a single element.
void que_peek(const queue_s queue, void * element, const size_t element_size);

/// @brief Dequeues element from the start of the queue.
/// @param queue Queue data structure.
/// @param element Single element to save dequeued element into.
/// @param element_size Size of a single element.
void que_dequeue(queue_s * queue, void * element, const size_t element_size);

/// @brief Iterates over each element in queue.
/// @param queue Queue data structure.
/// @param operate Fucntion pointer to perform a single operation on element in queue using arguments.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void que_foreach(queue_s const * queue, const operate_fn operate, const size_t element_size, void * arguments);

/// @brief Maps each element in queue into an array to manage.
/// @param queue Queue data structure.
/// @param manage Function pointer to manage an array of finite number of element in queue.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void que_map(queue_s const * queue, const manage_fn manage, const size_t element_size, void * arguments);

#endif // QUEUE_H
