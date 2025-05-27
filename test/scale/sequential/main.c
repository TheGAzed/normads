#include "unit.h"

GREATEST_MAIN_DEFS();

int main(const int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(scale_stack_unit_test);
    RUN_SUITE(scale_queue_unit_test);

    GREATEST_MAIN_END();
}