#include <iostream>
#include <sstream>
#include <string>

extern "C" {
    int PrimeCount(int A, int B);
    float E(int x);
}

int main() {
    std::string command;
    while (std::getline(std::cin, command)) {
        if (command.empty()) continue;
        std::istringstream iss(command);
        int cmd;
        iss >> cmd;
        if (cmd == 1) {
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
    return 0;
}
