#include <unit.h>

#include <helper/helper.h>

#include <scale/sequential/queue/squeue.h>

TEST CREATE_01(void) {
    squeue_s test = sque_create();

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST DESTROY_01(void) {
    squeue_s test = sque_create();
    sque_destroy(&test, destroy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    PASS();
}

TEST DESTROY_02(void) {
    squeue_s test = sque_create();
    const DATA_TYPE a = 42;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));
    sque_destroy(&test, destroy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    PASS();
}

TEST DESTROY_03(void) {
    squeue_s test = sque_create();
    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }
    sque_destroy(&test, destroy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    PASS();
}

TEST DESTROY_04(void) {
    squeue_s test = sque_create();
    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }
    sque_destroy(&test, destroy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    PASS();
}

TEST DESTROY_05(void) {
    squeue_s test = sque_create();
    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }
    sque_destroy(&test, destroy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    PASS();
}

TEST IS_FULL_01(void) {
    squeue_s test = sque_create();

    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be full", sque_is_full(test));

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_FULL_02(void) {
    squeue_s test = sque_create();

    const DATA_TYPE a = 42;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));
    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be full", sque_is_full(test));

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_FULL_03(void) {
    squeue_s test = sque_create();

    for (size_t i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }
    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be full", sque_is_full(test));

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_FULL_04(void) {
    squeue_s test = sque_create();

    for (size_t i = 0; i < REALLOC_CHUNK; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }
    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be full", sque_is_full(test));

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_FULL_05(void) {
    squeue_s test = sque_create();

    for (size_t i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }
    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be full", sque_is_full(test));

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PEEP_01(void) {
    squeue_s test = sque_create();

    const DATA_TYPE a = 42;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));

    DATA_TYPE b = 0;
    sque_peek(test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to peep 42", 42, b);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PEEP_02(void) {
    squeue_s test = sque_create();

    const DATA_TYPE a = 42;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));
    for (int i = 0; i < REALLOC_CHUNK - 2; ++i) {
        const DATA_TYPE a = -1;
        sque_enqueue(&test, &a, sizeof(DATA_TYPE));
    }

    DATA_TYPE b = 0;
    sque_peek(test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to peep 42", 42, b);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PEEP_03(void) {
    squeue_s test = sque_create();

    const DATA_TYPE a = 42;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));
    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        const DATA_TYPE a = -1;
        sque_enqueue(&test, &a, sizeof(DATA_TYPE));
    }

    DATA_TYPE b = 0;
    sque_peek(test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to peep 42", 42, b);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PEEP_04(void) {
    squeue_s test = sque_create();

    const DATA_TYPE a = 42;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));
    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        const DATA_TYPE a = -1;
        sque_enqueue(&test, &a, sizeof(DATA_TYPE));
    }

    DATA_TYPE b = 0;
    sque_peek(test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to peep 42", 42, b);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PEEP_05(void) {
    squeue_s test = sque_create();

    const DATA_TYPE a = 42;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));
    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        const DATA_TYPE a = -1;
        sque_enqueue(&test, &a, sizeof(DATA_TYPE));
    }

    DATA_TYPE b = 0;
    sque_peek(test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to peep 42", 42, b);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PUSH_01(void) {
    squeue_s test = sque_create();

    const DATA_TYPE a = 42;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));

    DATA_TYPE b = 0;
    sque_dequeue(&test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to pop 42", 42, b);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PUSH_02(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PUSH_03(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PUSH_04(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST POP_01(void) {
    squeue_s test = sque_create();

    const DATA_TYPE a = 42;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));

    DATA_TYPE b = 0;
    sque_dequeue(&test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to pop 42", 42, b);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST POP_02(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST POP_03(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST POP_04(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST COPY_01(void) {
    squeue_s test = sque_create();

    squeue_s copy = sque_copy(test, memcpy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected sizes to be equal", test.size, copy.size);
    ASSERT_EQm("[IRS-ERROR] Expected heads to be equal", test.elements, copy.elements);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    sque_destroy(&copy, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST COPY_02(void) {
    squeue_s test = sque_create();

    const DATA_TYPE a = 42;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));

    squeue_s copy = sque_copy(test, memcpy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected sizes to be equal", test.size, copy.size);
    ASSERT_NEQm("[IRS-ERROR] Expected heads to not be equal", test.elements, copy.elements);

    DATA_TYPE b = 0;
    sque_dequeue(&test, &b, sizeof(DATA_TYPE));

    DATA_TYPE c = 0;
    sque_dequeue(&copy, &c, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected elements to be equal", b, c);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    sque_destroy(&copy, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST COPY_03(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    squeue_s copy = sque_copy(test, memcpy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected sizes to be equal", test.size, copy.size);
    ASSERT_NEQm("[IRS-ERROR] Expected heads to not be equal", test.elements, copy.elements);

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));

        DATA_TYPE c = 0;
        sque_dequeue(&copy, &c, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected elements to be equal", b, c);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    sque_destroy(&copy, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST COPY_04(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    squeue_s copy = sque_copy(test, memcpy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected sizes to be equal", test.size, copy.size);
    ASSERT_NEQm("[IRS-ERROR] Expected heads to not be equal", test.elements, copy.elements);

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));

        DATA_TYPE c = 0;
        sque_dequeue(&copy, &c, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected elements to be equal", b, c);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    sque_destroy(&copy, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST COPY_05(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    squeue_s copy = sque_copy(test, memcpy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected sizes to be equal", test.size, copy.size);
    ASSERT_NEQm("[IRS-ERROR] Expected heads to not be equal", test.elements, copy.elements);

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));

        DATA_TYPE c = 0;
        sque_dequeue(&copy, &c, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected elements to be equal", b, c);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    sque_destroy(&copy, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_EMPTY_01(void) {
    squeue_s test = sque_create();

    ASSERTm("[IRS-ERROR] Expected stack to be empty", sque_is_empty(test));

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_EMPTY_02(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be empty", sque_is_empty(test));

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_EMPTY_03(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be empty", sque_is_empty(test));

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_EMPTY_04(void) {
    squeue_s test = sque_create();

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be empty", sque_is_empty(test));

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST FOREACH_01(void) {
    squeue_s test = sque_create();
    const DATA_TYPE a = 0;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));

    int inc = 5;
    sque_foreach(&test, increment, sizeof(DATA_TYPE), &inc);

    DATA_TYPE b = 0;
    sque_dequeue(&test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected ith element by 'i'.", inc + 0, b);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));

    PASS();
}

TEST FOREACH_02(void) {
    squeue_s test = sque_create();
    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    int inc = 5;
    sque_foreach(&test, increment, sizeof(DATA_TYPE), &inc);

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected ith element by 'i'.", inc + i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));

    PASS();
}

TEST FOREACH_03(void) {
    squeue_s test = sque_create();
    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    int inc = 5;
    sque_foreach(&test, increment, sizeof(DATA_TYPE), &inc);

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected ith element by 'i'.", inc + i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));

    PASS();
}

TEST FOREACH_04(void) {
    squeue_s test = sque_create();
    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    int inc = 5;
    sque_foreach(&test, increment, sizeof(DATA_TYPE), &inc);

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected ith element by 'i'.", i + inc, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));

    PASS();
}

TEST MAP_01(void) {
    squeue_s test = sque_create();

    sque_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare, }));

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_02(void) {
    squeue_s test = sque_create();

    const DATA_TYPE a = 42;
    sque_enqueue(&test, &a, sizeof(DATA_TYPE));

    sque_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare, }));

    DATA_TYPE b = 0;
    sque_dequeue(&test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop 42.", 42, b);

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_03(void) {
    squeue_s test = sque_create();

    for (int i = (REALLOC_CHUNK - 1) >> 1; i < REALLOC_CHUNK - 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK - 1) >> 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    sque_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare, }));

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_04(void) {
    squeue_s test = sque_create();

    for (int i = (REALLOC_CHUNK) >> 1; i < REALLOC_CHUNK; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK) >> 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    sque_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare, }));

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_05(void) {
    squeue_s test = sque_create();

    for (int i = (REALLOC_CHUNK + 1) >> 1; i < REALLOC_CHUNK + 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK + 1) >> 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    sque_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare, }));

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_06(void) {
    squeue_s test = sque_create();

    for (int i = (REALLOC_CHUNK - 1) >> 1; i < REALLOC_CHUNK - 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK - 1) >> 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    sque_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare_reverse, }));

    for (int i = REALLOC_CHUNK - 2; i >= 0; --i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_07(void) {
    squeue_s test = sque_create();

    for (int i = (REALLOC_CHUNK) >> 1; i < REALLOC_CHUNK; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK) >> 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    sque_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare_reverse, }));

    for (int i = REALLOC_CHUNK - 1; i >= 0; --i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_08(void) {
    squeue_s test = sque_create();

    for (int i = (REALLOC_CHUNK + 1) >> 1; i < REALLOC_CHUNK + 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK + 1) >> 1; ++i) {
        sque_enqueue(&test, &i, sizeof(DATA_TYPE));
    }

    sque_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare_reverse, }));

    for (int i = REALLOC_CHUNK; i >= 0; --i) {
        DATA_TYPE b = 0;
        sque_dequeue(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sque_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

SUITE (scale_queue_unit_test) {
    // create
    RUN_TEST(CREATE_01);
    // destroy
    RUN_TEST(DESTROY_01); RUN_TEST(DESTROY_02); RUN_TEST(DESTROY_03); RUN_TEST(DESTROY_04); RUN_TEST(DESTROY_05);
    // is full
    RUN_TEST(IS_FULL_01); RUN_TEST(IS_FULL_02); RUN_TEST(IS_FULL_03); RUN_TEST(IS_FULL_04); RUN_TEST(IS_FULL_05);
    // peep
    RUN_TEST(PEEP_01); RUN_TEST(PEEP_02); RUN_TEST(PEEP_03); RUN_TEST(PEEP_04); RUN_TEST(PEEP_05);
    // push
    RUN_TEST(PUSH_01); RUN_TEST(PUSH_02); RUN_TEST(PUSH_03); RUN_TEST(PUSH_04);
    // pop
    RUN_TEST(POP_01); RUN_TEST(POP_02); RUN_TEST(POP_03); RUN_TEST(POP_04);
    // copy
    RUN_TEST(COPY_01); RUN_TEST(COPY_02); RUN_TEST(COPY_03); RUN_TEST(COPY_04); RUN_TEST(COPY_05);
    // is empty
    RUN_TEST(IS_EMPTY_01); RUN_TEST(IS_EMPTY_02); RUN_TEST(IS_EMPTY_03); RUN_TEST(IS_EMPTY_04);
    // foreach
    RUN_TEST(FOREACH_01); RUN_TEST(FOREACH_02); RUN_TEST(FOREACH_03); RUN_TEST(FOREACH_04);
    // map
    RUN_TEST(MAP_01); RUN_TEST(MAP_02); RUN_TEST(MAP_03); RUN_TEST(MAP_04); RUN_TEST(MAP_05);
    RUN_TEST(MAP_06); RUN_TEST(MAP_07); RUN_TEST(MAP_08);
}
