
#include "parent.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <semaphore.h>
#include <fcntl.h>

void ParentProcess(const char* pathToChild, std::istream& streamIn, std::ostream& streamOut) {
    constexpr auto shm_name = "/shared_memory";
    constexpr size_t shm_size = 1024;

    char* shared_data = static_cast<char*>(CreateFileMapping(shm_name, shm_size));
    if (shared_data == MAP_FAILED) {
        perror("Parent: Ошибка создания общей памяти");
        exit(EXIT_FAILURE);
    }

    sem_t* sem_child = sem_open("/sem_child", O_CREAT, 0666, 0);
    sem_t* sem_parent = sem_open("/sem_parent", O_CREAT, 0666, 0);
    if (sem_child == SEM_FAILED || sem_parent == SEM_FAILED) {
        perror("Parent: Ошибка создания семафоров");
        CloseFileMapping(shm_name, shared_data, shm_size);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Parent: Ошибка fork");
        CloseFileMapping(shm_name, shared_data, shm_size);
        sem_close(sem_child);
        sem_close(sem_parent);
        sem_unlink("/sem_child");
        sem_unlink("/sem_parent");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execl(pathToChild, pathToChild, nullptr);
        perror("Child: Ошибка exec");
        exit(EXIT_FAILURE);
    } else {
        std::string line;
        while (true) {
            std::cout << "Введите строку с тремя числами (или 'exit' для выхода):\n";
            std::getline(streamIn, line);

            strncpy(shared_data, line.c_str(), shm_size);

            sem_post(sem_child);

            if (line == "exit") break;

            sem_wait(sem_parent);

            streamOut << shared_data << std::endl;
        }

        waitpid(pid, nullptr, 0);

        CloseFileMapping(shm_name, shared_data, shm_size);
        sem_close(sem_child);
        sem_close(sem_parent);
        sem_unlink("/sem_child");
        sem_unlink("/sem_parent");
    }
}
