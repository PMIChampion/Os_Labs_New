#include <utils.hpp>
#include <sstream>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

void* CreateFileMapping(const char* name, size_t size){
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, size);
    void* addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    return addr;
}

void CloseFileMapping(const char* name, void* addr, size_t size){
    munmap(addr, size);
    shm_unlink(name);
}

pid_t CreateChild(){
    if (pid_t pid = fork(); pid >= 0){
        return pid;
    }
    std::perror("Дочерний процесс не создан");
    exit(EXIT_FAILURE);
}

void Exec(const char * pathToChild){
    if (execl(pathToChild, pathToChild, nullptr) == -1){
        perror("Не исполняется exec");
        exit(EXIT_FAILURE);
    }
}