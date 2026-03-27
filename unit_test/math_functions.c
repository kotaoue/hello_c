#include "math_functions.h"

int is_odd(int n) {
    return n % 2 != 0;
}

int mod(int a, int b) {
    if (b == 0) {
        return 0;
    }
    return a % b;
}
