#include <parent.hpp>


void ParentProcess(const char * pathToChild, std::istream & streamIn, std::ostream & streamOut){
    int Parent_Child[2];
    CreatePipe(Parent_Child);

    int Child_Parent[2];
    CreatePipe(Child_Parent);

    pid_t pid = CreateChild();
    //Мы в дочернем процессе
    if(pid == 0){
        close(Parent_Child[1]);
        close(Child_Parent[0]);

        if (dup2(Parent_Child[0], STDIN_FILENO) == -1){
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }

        if (dup2(Child_Parent[1], STDOUT_FILENO) == -1){
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }
        close(Parent_Child[0]);  
        close(Child_Parent[1]);

        Exec("/Users/vladislavburdinskij/Documents/lab_OS/OS_labs/lab-1/build/child");
    }else{
        close(Parent_Child[0]);
        close(Child_Parent[1]);

        std::string line;
        std::getline(streamIn, line);
        line += "\n";
        write(Parent_Child[1], line.c_str(), line.size());
        close(Parent_Child[1]);

        char buffer[256];
        int bytesRead;
        while ((bytesRead = read(Child_Parent[0], buffer, sizeof(buffer))) > 0) {
            streamOut.write(buffer, bytesRead);
        }
        close(Child_Parent[0]);  // Закрываем конец для чтения

        // Ожидание завершения дочернего процесса
        int status;
        wait(&status);
    }
}
