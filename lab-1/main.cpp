#include "parent.hpp"


int main(void) {
    
    ParentProcess(getenv("PATH_TO_CHILD"), std::cin, std::cout);

    exit(EXIT_SUCCESS);
}