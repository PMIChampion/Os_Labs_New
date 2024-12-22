#include <iostream>
#include <dlfcn.h>    // Для macos
#include <sstream>
#include <string>

typedef int (*PrimeCountFunc)(int, int);
typedef float (*EFunc)(int);

int main() {
    void* handlePrime = dlopen("./prime_naive.dylib", RTLD_LAZY);
    void* handleE = dlopen("./e_formula.dylib", RTLD_LAZY);
    if (!handlePrime || !handleE) {
        std::cerr << "Ошибка загрузки библиотек" << std::endl;
        return 1;
    }

    PrimeCountFunc PrimeCount = (PrimeCountFunc)dlsym(handlePrime, "PrimeCount");
    EFunc E = (EFunc)dlsym(handleE, "E");

    std::string command;
    while (std::getline(std::cin, command)) {
        if (command.empty()) continue;
        std::istringstream iss(command);
        int cmd;
        iss >> cmd;
        if (cmd == 0) {
            dlclose(handlePrime);
            dlclose(handleE);
            static bool toggle = false;
            toggle = !toggle;
            if (toggle) {
                handlePrime = dlopen("./prime_sieve.dylib", RTLD_LAZY);
                handleE = dlopen("./e_series.dylib", RTLD_LAZY);
            } else {
                handlePrime = dlopen("./prime_naive.dylib", RTLD_LAZY);
                handleE = dlopen("./e_formula.dylib", RTLD_LAZY);
            }
            if (!handlePrime || !handleE) {
                std::cerr << "Ошибка загрузки библиотек" << std::endl;
                return 1;
            }
            PrimeCount = (PrimeCountFunc)dlsym(handlePrime, "PrimeCount");
            E = (EFunc)dlsym(handleE, "E");
            std::cout << "Реализации переключены" << std::endl;
        } else if (cmd == 1) {
            int A, B;
            iss >> A >> B;
            int result = PrimeCount(A, B);
            std::cout << "Количество простых чисел: " << result << std::endl;
        } else if (cmd == 2) {
            int x;
            iss >> x;
            float result = E(x);
            std::cout << "Значение числа e: " << result << std::endl;
        } else {
            std::cout << "Неизвестная команда" << std::endl;
        }
    }

    dlclose(handlePrime);
    dlclose(handleE);
    return 0;
}
