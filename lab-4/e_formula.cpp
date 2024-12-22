#include <cmath>
#include <iostream>

extern "C" float E(int x) {
    if (x == 0) return 1.0f;
    float result = pow(1.0f + 1.0f / x, x);
    return result;
}

