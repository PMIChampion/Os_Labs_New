#include "parent.hpp"
#include <iostream>
#include <cstdlib>

int main(void) {
    const char* pathToChild = getenv("WAY_TO_FILE");
    if (pathToChild == nullptr) {
        std::cerr << "Переменная WAY_TO_FILE не существует" << std::endl;
        exit(EXIT_FAILURE);
    }
    ParentProcess(pathToChild, std::cin, std::cout);
    exit(EXIT_SUCCESS);
}