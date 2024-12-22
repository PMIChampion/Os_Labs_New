#pragma once

#include <cstddef> 
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

void* CreateFileMapping(const char* name, size_t size);
void CloseFileMapping(const char* name, void* addr, size_t size);
pid_t CreateChild();
void Exec(const char * pathToChild);

struct SharedData {
    sem_t sem_parent;
    sem_t sem_child;
    char fileName[256];
    float number;
    float result;
    bool ready;
};