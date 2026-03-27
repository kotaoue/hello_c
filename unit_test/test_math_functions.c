#include <stdio.h>
#include <assert.h>
#include "math_functions.h"

static int passed = 0;
static int failed = 0;

#define TEST(name, expr) \
    do { \
        if (expr) { \
            printf("[PASS] %s\n", name); \
            passed++; \
        } else { \
            printf("[FAIL] %s\n", name); \
            failed++; \
        } \
    } while (0)

static void test_is_odd(void) {
    TEST("is_odd(1) == 1", is_odd(1) == 1);
    TEST("is_odd(2) == 0", is_odd(2) == 0);
    TEST("is_odd(3) == 1", is_odd(3) == 1);
    TEST("is_odd(0) == 0", is_odd(0) == 0);
    TEST("is_odd(-1) == 1", is_odd(-1) == 1);
    TEST("is_odd(-2) == 0", is_odd(-2) == 0);
}

static void test_mod(void) {
    TEST("mod(10, 3) == 1", mod(10, 3) == 1);
    TEST("mod(9, 3) == 0",  mod(9, 3) == 0);
    TEST("mod(7, 2) == 1",  mod(7, 2) == 1);
    TEST("mod(0, 5) == 0",  mod(0, 5) == 0);
    TEST("mod(1, 1) == 0",  mod(1, 1) == 0);
    TEST("mod(5, 0) == 0",  mod(5, 0) == 0);
}

int main(void) {
    printf("=== is_odd tests ===\n");
    test_is_odd();

    printf("\n=== mod tests ===\n");
    test_mod();

    printf("\nResult: %d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
