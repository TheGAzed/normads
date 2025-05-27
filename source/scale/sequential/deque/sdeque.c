#include <scale/sequential/deque/sdeque.h>

#include <string.h>

#ifndef ASSERT_SDEQ
#   include <assert.h>
#   define ASSERT_SDEQ assert
#endif

#if !defined(REALLOC_SDEQ) && !defined(FREE_SDEQ)
#   include <stdlib.h>
#   ifndef REALLOC_SDEQ
#       define REALLOC_SDEQ realloc
#   endif
#   ifndef FREE_SDEQ
#       define FREE_SDEQ free
#   endif
#elif !defined(REALLOC_SDEQ)
#   error Reallocator macro is not defined!
#elif !defined(FREE_SDEQ)
#   error Free macro is not defined!
#endif

#if !defined(IS_CAPACITY_SDEQ) && !defined(EXPAND_CAPACITY_SDEQ)
#   ifndef REALLOC_CHUNK_SDEQ
#       define REALLOC_CHUNK_SDEQ (1 << 5)
#   elif REALLOC_CHUNK_SDEQ <= 0
#       error 'REALLOC_CHUNK_SDEQ' cannot be less than or equal to 0
#   endif
#   define IS_CAPACITY_SDEQ(size) (!((size) % REALLOC_CHUNK_SDEQ)) // Checks if size has reached capacity.
#   define EXPAND_CAPACITY_SDEQ(capacity) ((capacity) + REALLOC_CHUNK_SDEQ) // Calculates next deque's capacity.
#elif !defined(IS_CAPACITY_SDEQ)
#   error Is capacity reached check is not defined.
#elif !defined(EXPAND_CAPACITY_SDEQ)
#   error Expand capacity size is not defined.
#endif

sdeque_s sdeq_create(void) {
    return (sdeque_s) { 0 };
}

void sdeq_destroy(sdeque_s * deque, const destroy_fn destroy, const size_t element_size) {
    ASSERT_SDEQ(deque && "[ERROR] 'deque' parameter is NULL.");
    ASSERT_SDEQ(destroy && "[ERROR] 'destroy' parameter is NULL.");
    ASSERT_SDEQ(element_size && "[ERROR] Element's size can't be zero.");

    // destroy elements from current index until capacity
    char * elements = (char*)deque->elements + (deque->current * element_size);
    const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
    for (size_t i = 0; i < right_size; ++i) {
        destroy(elements);
        elements += element_size;
    }

    // reset elements array to the beginning and subtract removed size from deque's and destroy the rest
    elements = deque->elements;
    for (size_t i = 0; i < deque->size - right_size; ++i) {
        destroy(elements);
        elements += element_size;
    }

    deque->size = deque->current = deque->capacity = 0;
    FREE_SDEQ(deque->elements);
    deque->elements = NULL;
}

sdeque_s sdeq_copy(const sdeque_s deque, const copy_fn copy, const size_t element_size) {
    ASSERT_SDEQ(copy && "[ERROR] 'copy' parameter is NULL.");
    ASSERT_SDEQ(element_size && "[ERROR] Element's size can't be zero.");

    sdeque_s deque_copy = { 0 };

    char const * elements = (char*)deque.elements + (deque.current * element_size);
    const size_t right_size = (deque.current + deque.size) > deque.capacity ? deque.capacity - deque.current : deque.size;
    for (size_t i = 0; i < right_size; ++i, deque_copy.size++) {
        if (IS_CAPACITY_SDEQ(deque_copy.size)) {
            const size_t expand = deque_copy.capacity = EXPAND_CAPACITY_SDEQ(deque_copy.size);

            deque_copy.elements = REALLOC_SDEQ(deque_copy.elements, element_size * expand);
            ASSERT_SDEQ(deque_copy.elements && "[ERROR] Memory allocation failed.");
        }

        copy((char*)deque_copy.elements + (deque_copy.size * element_size), elements, element_size);
        elements += element_size;
    }

    elements = deque.elements;
    for (size_t i = 0; i < deque.size - deque_copy.size; ++i) {
        if (IS_CAPACITY_SDEQ(deque_copy.size)) {
            const size_t expand = deque_copy.capacity = EXPAND_CAPACITY_SDEQ(deque_copy.size);

            deque_copy.elements = REALLOC_SDEQ(deque_copy.elements, element_size * expand);
            ASSERT_SDEQ(deque_copy.elements && "[ERROR] Memory allocation failed.");
        }

        copy((char*)deque_copy.elements + (deque_copy.size * element_size), elements, element_size);
        elements += element_size;
    }

    return deque_copy;
}

bool sdeq_is_full(const sdeque_s deque) {
    return !(~deque.size);
}

bool sdeq_is_empty(const sdeque_s deque) {
    return !(deque.size);
}

void sdeq_enqueue_front(sdeque_s * deque, const void * element, const size_t element_size) {
    ASSERT_SDEQ(deque && "[ERROR] 'deque' parameter is NULL.");
    ASSERT_SDEQ((~deque->size) && "[ERROR] Queue size variable will overflow.");
    ASSERT_SDEQ(element_size && "[ERROR] Element's size can't be zero.");

    if (IS_CAPACITY_SDEQ(deque->size)) {
        const size_t expand = EXPAND_CAPACITY_SDEQ(deque->size);

        void * temporary = REALLOC_SDEQ(NULL, element_size * expand);
        ASSERT_SDEQ(temporary && "[ERROR] Memory allocation failed.");

        const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
        memcpy(temporary, (char*)deque->elements + (deque->current * element_size), right_size * element_size);

        const size_t left_size = deque->size - right_size;
        memcpy((char*)temporary + (right_size * element_size), deque->elements, left_size * element_size);

        FREE_SDEQ(deque->elements);
        deque->elements = temporary;
        deque->capacity = expand;
        deque->current = 0;
    }

    const size_t next_front_index = (deque->current + deque->size) % deque->capacity;
    memcpy((char*)deque->elements + (next_front_index * element_size), element, element_size);
    deque->size++;
}

void sdeq_enqueue_rear(sdeque_s * deque, const void * element, const size_t element_size) {
    ASSERT_SDEQ(deque && "[ERROR] 'deque' parameter is NULL.");
    ASSERT_SDEQ((~deque->size) && "[ERROR] Queue size variable will overflow.");
    ASSERT_SDEQ(element_size && "[ERROR] Element's size can't be zero.");

    if (IS_CAPACITY_SDEQ(deque->size)) {
        const size_t expand = EXPAND_CAPACITY_SDEQ(deque->size);

        void * temporary = REALLOC_SDEQ(NULL, element_size * expand);
        ASSERT_SDEQ(temporary && "[ERROR] Memory allocation failed.");

        const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
        memcpy(temporary, (char*)deque->elements + (deque->current * element_size), right_size * element_size);

        const size_t left_size = deque->size - right_size;
        memcpy((char*)temporary + (right_size * element_size), deque->elements, left_size * element_size);

        FREE_SDEQ(deque->elements);
        deque->elements = temporary;
        deque->capacity = expand;
        deque->current = 0;
    }

    if (!deque->current) {
        deque->current = deque->capacity;
    }
    deque->current--;
    memcpy((char*)deque->elements + (deque->current * element_size), element, element_size);
    deque->size++;
}

void sdeq_peek_front(const sdeque_s deque, void * element, const size_t element_size) {
    ASSERT_SDEQ(deque.size && "[ERROR] Can't peek empty deque.");
    ASSERT_SDEQ(element && "[ERROR] 'element' parameter is NULL.");
    ASSERT_SDEQ(element_size && "[ERROR] Element's size can't be zero.");

    const size_t current_front_index = (deque.current + deque.size - 1) % deque.capacity;
    memcpy(element, (char*)deque.elements + (current_front_index * element_size), element_size);
}

void sdeq_peek_rear(const sdeque_s deque, void * element, const size_t element_size) {
    ASSERT_SDEQ(deque.size && "[ERROR] Can't peek empty deque.");
    ASSERT_SDEQ(element && "[ERROR] 'element' parameter is NULL.");
    ASSERT_SDEQ(element_size && "[ERROR] Element's size can't be zero.");

    memcpy(element, (char*)deque.elements + (deque.current * element_size), element_size);
}

void sdeq_dequeue_front(sdeque_s * deque, void * element, const size_t element_size) {
    ASSERT_SDEQ(deque && "[ERROR] 'deque' parameter is NULL.");
    ASSERT_SDEQ(deque->size && "[ERROR] Can't peep empty deque.");
    ASSERT_SDEQ(element && "[ERROR] 'element' parameter is NULL.");
    ASSERT_SDEQ(element_size && "[ERROR] Element's size can't be zero.");

    const size_t current_front_index = (deque->current + deque->size - 1) % deque->capacity;
    memcpy(element, (char*)deque->elements + (current_front_index * element_size), element_size);
    deque->size--;

    if (!(IS_CAPACITY_SDEQ(deque->size))) {
        return;
    }

    if (deque->size) {
        void * temporary = REALLOC_SDEQ(NULL, element_size * deque->size);
        ASSERT_SDEQ(temporary && "[ERROR] Memory allocation failed.");

        const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
        memcpy(temporary, (char*)deque->elements + (deque->current * element_size), right_size * element_size);

        const size_t left_size = deque->size - right_size;
        memcpy((char*)temporary + (right_size * element_size), deque->elements, left_size * element_size);

        FREE_SDEQ(deque->elements);
        deque->elements = temporary;
        deque->capacity = deque->size;
        deque->current = 0;
    } else {
        FREE_SDEQ(deque->elements);
        deque->elements = NULL;
    }
}

void sdeq_dequeue_rear(sdeque_s * deque, void * element, const size_t element_size) {
    ASSERT_SDEQ(deque && "[ERROR] 'deque' parameter is NULL.");
    ASSERT_SDEQ(deque->size && "[ERROR] Can't peep empty deque.");
    ASSERT_SDEQ(element && "[ERROR] 'element' parameter is NULL.");
    ASSERT_SDEQ(element_size && "[ERROR] Element's size can't be zero.");

    memcpy(element, (char*)deque->elements + (deque->current * element_size), element_size);
    deque->capacity++;
    deque->size--;
    if (deque->capacity == deque->current) {
        deque->current = 0;
    }

    if (!(IS_CAPACITY_SDEQ(deque->size))) {
        return;
    }

    if (deque->size) {
        void * temporary = REALLOC_SDEQ(NULL, element_size * deque->size);
        ASSERT_SDEQ(temporary && "[ERROR] Memory allocation failed.");

        const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
        memcpy(temporary, (char*)deque->elements + (deque->current * element_size), right_size * element_size);

        const size_t left_size = deque->size - right_size;
        memcpy((char*)temporary + (right_size * element_size), deque->elements, left_size * element_size);

        FREE_SDEQ(deque->elements);
        deque->elements = temporary;
        deque->capacity = deque->size;
        deque->current = 0;
    } else {
        FREE_SDEQ(deque->elements);
        deque->elements = NULL;
    }
}

void sdeq_foreach(sdeque_s const * deque, const operate_fn operate, const size_t element_size, void * arguments) {
    ASSERT_SDEQ(deque && "[ERROR] 'deque' parameter is NULL.");
    ASSERT_SDEQ(operate && "[ERROR] 'operate' parameter is NULL.");
    ASSERT_SDEQ(element_size && "[ERROR] Element's size can't be zero.");

    char * elements = (char*)deque->elements + (deque->current * element_size);
    const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
    for (size_t i = 0; i < right_size && operate(elements, element_size, arguments); ++i) {
        elements += element_size;
    }

    elements = deque->elements;
    for (size_t i = 0; i < deque->size - right_size && operate(elements, element_size, arguments); ++i) {
        elements += element_size;
    }
}

void sdeq_map(sdeque_s const * deque, const manage_fn manage, const size_t element_size, void * arguments) {
    ASSERT_SDEQ(deque && "[ERROR] 'deque' parameter is NULL.");
    ASSERT_SDEQ(manage && "[ERROR] 'manage' parameter is NULL.");
    ASSERT_SDEQ(element_size && "[ERROR] Element's size can't be zero.");

    // create temporary array to have elements in order
    void * elements_array = REALLOC_SDEQ(NULL, deque->size * element_size);
    ASSERT_SDEQ((!(deque->size) || elements_array) && "[ERROR] Memory allocation failed.");

    // copy deque's elements into temporary array
    char * elements = (char*)deque->elements + (deque->current * element_size);
    const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
    memcpy(elements_array, elements, right_size * element_size);
    memcpy((char*)elements_array + (right_size * element_size), deque->elements, deque->size - right_size);

    manage(elements_array, deque->size, element_size, arguments);

    // copy managed elements back into deque
    memcpy(elements, elements_array, right_size * element_size);
    memcpy(deque->elements, (char*)elements_array + (right_size * element_size), deque->size - right_size);

    FREE_SDEQ(elements_array);
}
