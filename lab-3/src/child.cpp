#include "child.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <semaphore.h>
#include <fcntl.h>
#include <cstring>

int calculation(int num1, int num2, int num3) {
    if (num2 == 0 || num3 == 0) throw std::runtime_error("Деление на ноль");
    return num1 / num2 / num3;
}

int main() {
    constexpr auto shm_name = "/shared_memory";
    constexpr size_t shm_size = 1024;

    char* shared_data = static_cast<char*>(CreateFileMapping(shm_name, shm_size));
    if (shared_data == MAP_FAILED) {
        perror("Child: Ошибка подключения к общей памяти");
        exit(EXIT_FAILURE);
    }

    sem_t* sem_child = sem_open("/sem_child", 0);
    sem_t* sem_parent = sem_open("/sem_parent", 0);
    if (sem_child == SEM_FAILED || sem_parent == SEM_FAILED) {
        perror("Child: Ошибка открытия семафоров");
        CloseFileMapping(shm_name, shared_data, shm_size);
        exit(EXIT_FAILURE);
    }

    while (true) {
        sem_wait(sem_child);

        std::string input(shared_data);

        if (input == "exit") {
            sem_post(sem_parent);
            break;
        }

        std::stringstream ss(input);
        int num1, num2, num3;
        std::string result;

        if (ss >> num1 >> num2 >> num3) {
            try {
                int res = calculation(num1, num2, num3);
                result = "Результат: " + std::to_string(res);
            } catch (const std::exception& e) {
                result = e.what();
            }
        } else {
            result = "Некорректный ввод";
        }

        strncpy(shared_data, result.c_str(), shm_size);

        sem_post(sem_parent);
    }

    CloseFileMapping(shm_name, shared_data, shm_size);
    sem_close(sem_child);
    sem_close(sem_parent);

    return 0;
}
