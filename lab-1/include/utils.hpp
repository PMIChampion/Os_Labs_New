#pragma once

#include <sstream>
#include <algorithm>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>

enum PipeEnd {
    READ_END,
    WRITE_END
};

void                CreatePipe(int pipeFd[2]);
pid_t               CreateChild();
void                Exec(const char * pathToChild);
