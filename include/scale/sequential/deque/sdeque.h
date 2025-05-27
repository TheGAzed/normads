#ifndef SDEQUE_H
#define SDEQUE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct sdeque {
    void * elements; // array of elements
    size_t size, current, capacity; // size, current index and capacity of deque
} sdeque_s;

/// @brief Function pointer to destroy a single element in data structure. Based on 'free';
typedef void   (*destroy_fn) (void * element);
/// @brief Function pointer to copy a single element in data structure. Based on 'memcpy' and 'memmove'.
typedef void * (*copy_fn) (void * dest, const void * src, size_t size);
/// @brief Fucntion pointer to perform a single operation on element in data structure.
typedef bool   (*operate_fn) (void * element, size_t size, void * args);
/// @brief Function pointer to manage an array of finite number of element in data structure.
typedef void   (*manage_fn) (void * base, size_t n, size_t size, void * arg);

/// @brief Creates empty deque.
/// @return Empty deque structure.
sdeque_s sdeq_create(void);

/// @brief Destroys a deque.
/// @param deque Deque data structure.
/// @param destroy Function pointer to destroy a single element in deque.
/// @param element_size Size of a single element.
void sdeq_destroy(sdeque_s * deque, const destroy_fn destroy, const size_t element_size);

/// @brief Creates a copy of deque and its elements.
/// @param deque Deque data structure.
/// @param copy Function pointer to copy a single element in deque.
/// @param element_size Size of a single element.
/// @return A copy of the deque.
sdeque_s sdeq_copy(const sdeque_s deque, const copy_fn copy, const size_t element_size);

/// @brief Checks if deque size will overflow.
/// @param deque Deque data structure.
/// @return 'true' if deque size will overflow, 'false' otherwise.
bool sdeq_is_full(const sdeque_s deque);

/// @brief Checks if deque is empty.
/// @param deque Deque data structure.
/// @return 'true' if deque is empty, 'false' otherwise.
bool sdeq_is_empty(const sdeque_s deque);

/// @brief Enqueues element to the front of the deque.
/// @param deque Deque data structure.
/// @param element Single element to enqueue.
/// @param element_size Size of a single element.
void sdeq_enqueue_front(sdeque_s * deque, const void * element, const size_t element_size);

/// @brief Enqueues element to the rear of the deque.
/// @param deque Deque data structure.
/// @param element Single element to enqueue.
/// @param element_size Size of a single element.
void sdeq_enqueue_rear(sdeque_s * deque, const void * element, const size_t element_size);

/// @brief Peeks the front of the deque.
/// @param deque Deque data structure.
/// @param element Single element to save peeked element into.
/// @param element_size Size of a single element.
void sdeq_peek_front(const sdeque_s deque, void * element, const size_t element_size);

/// @brief Peeks the rear of the deque.
/// @param deque Deque data structure.
/// @param element Single element to save peeked element into.
/// @param element_size Size of a single element.
void sdeq_peek_rear(const sdeque_s deque, void * element, const size_t element_size);

/// @brief Dequeues element from the front of the deque.
/// @param deque Deque data structure.
/// @param element Single element to save dequeued element into.
/// @param element_size Size of a single element.
void sdeq_dequeue_front(sdeque_s * deque, void * element, const size_t element_size);

/// @brief Dequeues element from the rear of the deque.
/// @param deque Deque data structure.
/// @param element Single element to save dequeued element into.
/// @param element_size Size of a single element.
void sdeq_dequeue_rear(sdeque_s * deque, void * element, const size_t element_size);

/// @brief Iterates over each element in deque.
/// @param deque Deque data structure.
/// @param operate Fucntion pointer to perform a single operation on element in deque using arguments.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void sdeq_foreach(sdeque_s const * deque, const operate_fn operate, const size_t element_size, void * arguments);

/// @brief Maps each element in deque into an array to manage.
/// @param deque Deque data structure.
/// @param manage Function pointer to manage an array of finite number of element in deque.
/// @param element_size Size of a single element.
/// @param arguments Generic arguments for function pointer.
void sdeq_map(sdeque_s const * deque, const manage_fn manage, const size_t element_size, void * arguments);

#endif // SDEQUE_H
