#include "greet.h"
#include "clock.h"

const char *get_greeting(void) {
    int hour = get_current_hour();
    return get_greeting_by_hour(hour);
}

const char *get_greeting_by_hour(int hour) {
    if (hour >= 5 && hour < 12) {
        return "Good morning!";
    } else if (hour >= 12 && hour < 18) {
        return "Good afternoon!";
    } else {
        return "Good evening!";
    }
}
