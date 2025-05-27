#include <helper/helper.h>

#include <stdio.h>
#include <stdlib.h>

void destroy(void * element) {
    DATA_TYPE * convert = element;
    (*convert) = (DATA_TYPE) { 0 };
}

bool increment(void * element, const size_t size, void * args) {
    (void)(size);
    DATA_TYPE * convert = element;
    int * arg = args;
    (*convert) += (*arg);

    return true;
}

bool print(void * element, const size_t size, void * format) {
    (void)(size);
    DATA_TYPE * convert = element;
    fflush(stdout);
    printf(format, convert);

    return true;
}

int compare(const void * a, const void * b) {
    const DATA_TYPE * convert_a = a;
    const DATA_TYPE * convert_b = b;

    return (*convert_a) - (*convert_b);
}

int compare_reverse(const void * a, const void * b) {
    const DATA_TYPE * convert_a = a;
    const DATA_TYPE * convert_b = b;

    return (*convert_b) - (*convert_a);
}

void manage(void * elements, const size_t n, const size_t size, void * args) {
    function_ptr * arg = args;
    qsort(elements, n, size, arg->compare);
}
