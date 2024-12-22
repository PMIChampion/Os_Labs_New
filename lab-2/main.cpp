#include "include/my_solution.h"
#include <chrono>

std::vector<std::vector<int>> matrix;
std::vector<std::vector<int>> result_matrix;
int max_threads = 4;
std::string mode = "erosion";
int matrix_size = 100;

int main(int argc, char* argv[]){
    int opt;
    while ((opt = getopt(argc, argv, "t:m:n:")) != -1) {
    switch (opt) {
        case 't':
            max_threads = atoi(optarg);
            break;
        case 'm':
            mode = optarg;
            break;
        case 'n':
            matrix_size = atoi(optarg);
            break;
        default:
            std::cerr << "Использование: " << argv[0] << " -t max_threads -m mode (erosion/dilation)\n";
            exit(EXIT_FAILURE);
        }
    }

    matrix.resize(matrix_size, std::vector<int>(matrix_size));
    result_matrix.resize(matrix_size, std::vector<int>(matrix_size));
    srand(time(NULL));
    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            matrix[i][j] = rand() % 2;
        }
    }
    std::cout << "Матрица стартовая:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    int num_threads = max_threads < matrix_size ? max_threads : matrix_size;
    int rows_per_thread = matrix_size / num_threads;
    
    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData> thread_data(num_threads);
    int current_row = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].start_row = current_row;
        if (i == num_threads - 1) {
            thread_data[i].end_row = matrix_size - 1;
        } else {
            thread_data[i].end_row = current_row + rows_per_thread - 1;
        }
        thread_data[i].rows = matrix_size;  
        thread_data[i].cols = matrix_size;
        thread_data[i].width = matrix_size;
        thread_data[i].matrix = &matrix;
        thread_data[i].result_matrix = &result_matrix;
        if (mode == "erosion") {
            pthread_create(&threads[i], NULL, matrix_erosion, &thread_data[i]);
        } else if (mode == "dilatation") {
            pthread_create(&threads[i], NULL, matrix_dilatation, &thread_data[i]);
        }
        current_row = thread_data[i].end_row + 1;
    }


    for (int i = 0; i < num_threads; ++i) {
    pthread_join(threads[i], NULL);
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    // Вычисление затраченного времени
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "Результирующая матрица:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            std::cout << result_matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Время выполнения операции: " << elapsed.count() << " секунд" << std::endl;
    return 0;
}