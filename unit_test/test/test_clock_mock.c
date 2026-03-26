/**
 * test_clock_mock.c
 * CMock を使ったモックテストの例
 *
 * get_greeting() は内部で get_current_hour() を呼び出す。
 * CMock が生成した Mockclock を使って get_current_hour() をモックし、
 * 実際の時刻に依存せずテストする。
 */

#include "unity.h"
#include "Mockclock.h"
#include "greet.h"

void setUp(void) {
    Mockclock_Init();
}

void tearDown(void) {
    Mockclock_Verify();
    Mockclock_Destroy();
}

/* 時刻が午前9時の場合 → "Good morning!" */
void test_get_greeting_returns_morning_when_hour_is_9(void) {
    get_current_hour_ExpectAndReturn(9);
    TEST_ASSERT_EQUAL_STRING("Good morning!", get_greeting());
}

/* 時刻が午後3時の場合 → "Good afternoon!" */
void test_get_greeting_returns_afternoon_when_hour_is_15(void) {
    get_current_hour_ExpectAndReturn(15);
    TEST_ASSERT_EQUAL_STRING("Good afternoon!", get_greeting());
}

/* 時刻が午後10時の場合 → "Good evening!" */
void test_get_greeting_returns_evening_when_hour_is_22(void) {
    get_current_hour_ExpectAndReturn(22);
    TEST_ASSERT_EQUAL_STRING("Good evening!", get_greeting());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_greeting_returns_morning_when_hour_is_9);
    RUN_TEST(test_get_greeting_returns_afternoon_when_hour_is_15);
    RUN_TEST(test_get_greeting_returns_evening_when_hour_is_22);
    return UNITY_END();
}
