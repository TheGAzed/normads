#include <unit.h>

#include <helper/helper.h>

#include <scale/sequential/stack/sstack.h>

TEST CREATE_01(void) {
    sstack_s test = sstk_create();

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST DESTROY_01(void) {
    sstack_s test = sstk_create();
    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    PASS();
}

TEST DESTROY_02(void) {
    sstack_s test = sstk_create();
    const DATA_TYPE a = 42;
    sstk_push(&test, &a, sizeof(DATA_TYPE));
    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    PASS();
}

TEST DESTROY_03(void) {
    sstack_s test = sstk_create();
    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }
    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    PASS();
}

TEST DESTROY_04(void) {
    sstack_s test = sstk_create();
    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }
    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    PASS();
}

TEST DESTROY_05(void) {
    sstack_s test = sstk_create();
    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }
    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Test stack size is not zero.", 0, test.size);
    ASSERT_EQm("[IRS-ERROR] Test stack head is not NULL.", NULL, test.elements);

    PASS();
}

TEST IS_FULL_01(void) {
    sstack_s test = sstk_create();

    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be full", sstk_is_full(test));

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_FULL_02(void) {
    sstack_s test = sstk_create();

    const DATA_TYPE a = 42;
    sstk_push(&test, &a, sizeof(DATA_TYPE));
    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be full", sstk_is_full(test));

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_FULL_03(void) {
    sstack_s test = sstk_create();

    for (size_t i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }
    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be full", sstk_is_full(test));

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_FULL_04(void) {
    sstack_s test = sstk_create();

    for (size_t i = 0; i < REALLOC_CHUNK; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }
    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be full", sstk_is_full(test));

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_FULL_05(void) {
    sstack_s test = sstk_create();

    for (size_t i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }
    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be full", sstk_is_full(test));

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PEEP_01(void) {
    sstack_s test = sstk_create();

    const DATA_TYPE a = 42;
    sstk_push(&test, &a, sizeof(DATA_TYPE));

    DATA_TYPE b = 0;
    sstk_peep(test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to peep 42", 42, b);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PEEP_02(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK - 2; ++i) {
        const DATA_TYPE a = -1;
        sstk_push(&test, &a, sizeof(DATA_TYPE));
    }
    const DATA_TYPE a = 42;
    sstk_push(&test, &a, sizeof(DATA_TYPE));

    DATA_TYPE b = 0;
    sstk_peep(test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to peep 42", 42, b);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PEEP_03(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        const DATA_TYPE a = -1;
        sstk_push(&test, &a, sizeof(DATA_TYPE));
    }
    const DATA_TYPE a = 42;
    sstk_push(&test, &a, sizeof(DATA_TYPE));

    DATA_TYPE b = 0;
    sstk_peep(test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to peep 42", 42, b);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PEEP_04(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        const DATA_TYPE a = -1;
        sstk_push(&test, &a, sizeof(DATA_TYPE));
    }
    const DATA_TYPE a = 42;
    sstk_push(&test, &a, sizeof(DATA_TYPE));

    DATA_TYPE b = 0;
    sstk_peep(test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to peep 42", 42, b);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PEEP_05(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        const DATA_TYPE a = -1;
        sstk_push(&test, &a, sizeof(DATA_TYPE));
    }
    const DATA_TYPE a = 42;
    sstk_push(&test, &a, sizeof(DATA_TYPE));

    DATA_TYPE b = 0;
    sstk_peep(test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to peep 42", 42, b);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PUSH_01(void) {
    sstack_s test = sstk_create();

    const DATA_TYPE a = 42;
    sstk_push(&test, &a, sizeof(DATA_TYPE));

    DATA_TYPE b = 0;
    sstk_pop(&test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to pop 42", 42, b);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PUSH_02(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = REALLOC_CHUNK - 2; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PUSH_03(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = REALLOC_CHUNK - 1; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST PUSH_04(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = REALLOC_CHUNK; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST POP_01(void) {
    sstack_s test = sstk_create();

    const DATA_TYPE a = 42;
    sstk_push(&test, &a, sizeof(DATA_TYPE));

    DATA_TYPE b = 0;
    sstk_pop(&test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected to pop 42", 42, b);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST POP_02(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = REALLOC_CHUNK - 2; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST POP_03(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = REALLOC_CHUNK - 1; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST POP_04(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = REALLOC_CHUNK; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected to pop i", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST COPY_01(void) {
    sstack_s test = sstk_create();

    sstack_s copy = sstk_copy(test, memcpy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected sizes to be equal", test.size, copy.size);
    ASSERT_EQm("[IRS-ERROR] Expected heads to be equal", test.elements, copy.elements);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    sstk_destroy(&copy, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST COPY_02(void) {
    sstack_s test = sstk_create();

    const DATA_TYPE a = 42;
    sstk_push(&test, &a, sizeof(DATA_TYPE));

    sstack_s copy = sstk_copy(test, memcpy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected sizes to be equal", test.size, copy.size);
    ASSERT_NEQm("[IRS-ERROR] Expected heads to not be equal", test.elements, copy.elements);

    DATA_TYPE b = 0;
    sstk_pop(&test, &b, sizeof(DATA_TYPE));

    DATA_TYPE c = 0;
    sstk_pop(&copy, &c, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected elements to be equal", b, c);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    sstk_destroy(&copy, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST COPY_03(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    sstack_s copy = sstk_copy(test, memcpy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected sizes to be equal", test.size, copy.size);
    ASSERT_NEQm("[IRS-ERROR] Expected heads to not be equal", test.elements, copy.elements);

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));

        DATA_TYPE c = 0;
        sstk_pop(&copy, &c, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected elements to be equal", b, c);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    sstk_destroy(&copy, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST COPY_04(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    sstack_s copy = sstk_copy(test, memcpy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected sizes to be equal", test.size, copy.size);
    ASSERT_NEQm("[IRS-ERROR] Expected heads to not be equal", test.elements, copy.elements);

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));

        DATA_TYPE c = 0;
        sstk_pop(&copy, &c, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected elements to be equal", b, c);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    sstk_destroy(&copy, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST COPY_05(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    sstack_s copy = sstk_copy(test, memcpy, sizeof(DATA_TYPE));

    ASSERT_EQm("[IRS-ERROR] Expected sizes to be equal", test.size, copy.size);
    ASSERT_NEQm("[IRS-ERROR] Expected heads to not be equal", test.elements, copy.elements);

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));

        DATA_TYPE c = 0;
        sstk_pop(&copy, &c, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected elements to be equal", b, c);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    sstk_destroy(&copy, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_EMPTY_01(void) {
    sstack_s test = sstk_create();

    ASSERTm("[IRS-ERROR] Expected stack to be empty", sstk_is_empty(test));

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_EMPTY_02(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be empty", sstk_is_empty(test));

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_EMPTY_03(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be empty", sstk_is_empty(test));

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST IS_EMPTY_04(void) {
    sstack_s test = sstk_create();

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    ASSERT_FALSEm("[IRS-ERROR] Expected stack to not be empty", sstk_is_empty(test));

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST FOREACH_01(void) {
    sstack_s test = sstk_create();
    const DATA_TYPE a = 0;
    sstk_push(&test, &a, sizeof(DATA_TYPE));

    int inc = 5;
    sstk_foreach(&test, increment, sizeof(DATA_TYPE), &inc);

    DATA_TYPE b = 0;
    sstk_pop(&test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected ied element by 'i'.", inc + 0, b);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));

    PASS();
}

TEST FOREACH_02(void) {
    sstack_s test = sstk_create();
    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    int inc = 5;
    sstk_foreach(&test, increment, sizeof(DATA_TYPE), &inc);

    for (int i = REALLOC_CHUNK - 2; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected ied element by 'i'.", inc + i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));

    PASS();
}

TEST FOREACH_03(void) {
    sstack_s test = sstk_create();
    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    int inc = 5;
    sstk_foreach(&test, increment, sizeof(DATA_TYPE), &inc);

    for (int i = REALLOC_CHUNK - 1; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected ied element by 'i'.", inc + i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));

    PASS();
}

TEST FOREACH_04(void) {
    sstack_s test = sstk_create();
    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    int inc = 5;
    sstk_foreach(&test, increment, sizeof(DATA_TYPE), &inc);

    for (int i = REALLOC_CHUNK; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected ied element by 'i'.", i + inc, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));

    PASS();
}

TEST MAP_01(void) {
    sstack_s test = sstk_create();

    sstk_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare, }));

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_02(void) {
    sstack_s test = sstk_create();

    const DATA_TYPE a = 42;
    sstk_push(&test, &a, sizeof(DATA_TYPE));

    sstk_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare, }));

    DATA_TYPE b = 0;
    sstk_pop(&test, &b, sizeof(DATA_TYPE));
    ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop 42.", 42, b);

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_03(void) {
    sstack_s test = sstk_create();

    for (int i = (REALLOC_CHUNK - 1) >> 1; i < REALLOC_CHUNK - 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK - 1) >> 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    sstk_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare, }));

    for (int i = REALLOC_CHUNK - 2; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_04(void) {
    sstack_s test = sstk_create();

    for (int i = (REALLOC_CHUNK) >> 1; i < REALLOC_CHUNK; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK) >> 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    sstk_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare, }));

    for (int i = REALLOC_CHUNK - 1; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_05(void) {
    sstack_s test = sstk_create();

    for (int i = (REALLOC_CHUNK + 1) >> 1; i < REALLOC_CHUNK + 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK + 1) >> 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    sstk_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare, }));

    for (int i = REALLOC_CHUNK; i >= 0; --i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_06(void) {
    sstack_s test = sstk_create();

    for (int i = (REALLOC_CHUNK - 1) >> 1; i < REALLOC_CHUNK - 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK - 1) >> 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    sstk_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare_reverse, }));

    for (int i = 0; i < REALLOC_CHUNK - 1; ++i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_07(void) {
    sstack_s test = sstk_create();

    for (int i = (REALLOC_CHUNK) >> 1; i < REALLOC_CHUNK; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK) >> 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    sstk_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare_reverse, }));

    for (int i = 0; i < REALLOC_CHUNK; ++i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

TEST MAP_08(void) {
    sstack_s test = sstk_create();

    for (int i = (REALLOC_CHUNK + 1) >> 1; i < REALLOC_CHUNK + 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    for (int i = 0; i < (REALLOC_CHUNK + 1) >> 1; ++i) {
        sstk_push(&test, &i, sizeof(DATA_TYPE));
    }

    sstk_map(&test, manage, sizeof(DATA_TYPE), &((function_ptr) { .compare = compare_reverse, }));

    for (int i = 0; i < REALLOC_CHUNK + 1; ++i) {
        DATA_TYPE b = 0;
        sstk_pop(&test, &b, sizeof(DATA_TYPE));
        ASSERT_EQm("[IRS-ERROR] Expected managed stack to pop i.", i, b);
    }

    sstk_destroy(&test, destroy, sizeof(DATA_TYPE));
    PASS();
}

SUITE (scale_stack_unit_test) {
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
