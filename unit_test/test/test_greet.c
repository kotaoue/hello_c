/**
 * test_greet.c
 * Unity を使った単体テストの例
 *
 * get_greeting_by_hour() 関数のみをテストする。
 * 外部依存（時刻取得）がないため、モックは不要。
 */

#include "unity.h"
#include "greet.h"

void setUp(void) {}
void tearDown(void) {}

void test_get_greeting_by_hour_morning(void) {
    TEST_ASSERT_EQUAL_STRING("Good morning!", get_greeting_by_hour(6));
    TEST_ASSERT_EQUAL_STRING("Good morning!", get_greeting_by_hour(11));
}

void test_get_greeting_by_hour_afternoon(void) {
    TEST_ASSERT_EQUAL_STRING("Good afternoon!", get_greeting_by_hour(12));
    TEST_ASSERT_EQUAL_STRING("Good afternoon!", get_greeting_by_hour(17));
}

void test_get_greeting_by_hour_evening(void) {
    TEST_ASSERT_EQUAL_STRING("Good evening!", get_greeting_by_hour(18));
    TEST_ASSERT_EQUAL_STRING("Good evening!", get_greeting_by_hour(0));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_greeting_by_hour_morning);
    RUN_TEST(test_get_greeting_by_hour_afternoon);
    RUN_TEST(test_get_greeting_by_hour_evening);
    return UNITY_END();
}
