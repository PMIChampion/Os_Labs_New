#include <utils.hpp>


void CreatePipe(int pipeFd[2]){
    if (pipe(pipeFd) < 0){
        std::perror("Pipe не создается");
        exit(EXIT_FAILURE);
    }
}

pid_t CreateChild(){
    if (pid_t pid = fork(); pid >= 0){
        return pid;
    }
    std::perror("Дочерний процесс не создан");
    exit(EXIT_FAILURE);
}

void Exec(const char * pathToChild){
    pathToChild = "/Users/vladislavburdinskij/Documents/lab_OS/OS_labs/lab-1/build/child";
    if (execl(pathToChild, pathToChild, nullptr) == -1){
        perror("Не исполняется exec");
        exit(EXIT_FAILURE);
    }
}