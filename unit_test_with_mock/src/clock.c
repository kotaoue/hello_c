#include "clock.h"
#include <time.h>

int get_current_hour(void) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    return t->tm_hour;
}
