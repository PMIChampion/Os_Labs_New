#pragma once

#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#include <ctime>
#include <cstdlib>

struct ThreadData {
    const std::vector<std::vector<int>>* matrix;
    std::vector<std::vector<int>>* result_matrix;
    int start_row;
    int end_row;
    int width;
    int rows;
    int cols;
};

extern const int KERNEL_SIZE;
extern pthread_mutex_t mtx;
extern std::vector<std::vector<int>> filter;

void* matrix_erosion(void* arg);
void* matrix_dilatation(void* arg);

void perform_erosion(const std::vector<std::vector<int>>& input_matrix,
                     std::vector<std::vector<int>>& output_matrix,
                     const std::vector<std::vector<int>>& filter);

void perform_dilation(const std::vector<std::vector<int>>& input_matrix,
                      std::vector<std::vector<int>>& output_matrix,
                      const std::vector<std::vector<int>>& filter);
