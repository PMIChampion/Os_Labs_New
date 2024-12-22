#include <cmath>
#include <iostream>

extern "C" int PrimeCount(int A, int B) {
    int count = 0;
    for (int num = A; num <= B; ++num) {
        if (num < 2) continue;
        bool isPrime = true;
        int stop_point = sqrt(num);
        for (int j = 2; j <= stop_point; ++j) {
            if (num % j == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            ++count;
        }
    }
    return count;
}
