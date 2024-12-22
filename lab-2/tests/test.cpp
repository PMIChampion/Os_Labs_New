#include <gtest/gtest.h>
#include <pthread.h>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>

#include "../include/my_solution.h"

extern const int KERNEL_SIZE;
extern pthread_mutex_t mtx;
extern std::vector<std::vector<int>> filter;

void* matrix_erosion(void* arg);
void* matrix_dilatation(void* arg);

void singleThreadErosion(const std::vector<std::vector<int>>& input_matrix, std::vector<std::vector<int>>& output_matrix) {
    ThreadData data;
    data.start_row = 0;
    data.end_row = input_matrix.size() - 1;
    data.rows = input_matrix.size();
    data.cols = input_matrix[0].size();
    data.width = input_matrix[0].size();
    data.matrix = &input_matrix;
    data.result_matrix = &output_matrix;
    matrix_erosion(static_cast<void*>(&data));
}
void singleThreadDilatation(const std::vector<std::vector<int>>& input_matrix, std::vector<std::vector<int>>& output_matrix) {
    ThreadData data;
    data.start_row = 0;
    data.end_row = input_matrix.size() - 1;
    data.rows = input_matrix.size();
    data.cols = input_matrix[0].size();
    data.width = input_matrix[0].size();
    data.matrix = &input_matrix;
    data.result_matrix = &output_matrix;
    matrix_dilatation(static_cast<void*>(&data));
}

void multiThreadErosion(const std::vector<std::vector<int>>& input_matrix, std::vector<std::vector<int>>& output_matrix, int num_threads) {
    int matrix_size = input_matrix.size();
    int rows_per_thread = matrix_size / num_threads;
    int remainder_rows = matrix_size % num_threads;

    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData> thread_data(num_threads);

    int current_row = 0;

    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].start_row = current_row;
        thread_data[i].end_row = current_row + rows_per_thread - 1;
        if (i < remainder_rows) {
            thread_data[i].end_row += 1;
        }
        thread_data[i].rows = matrix_size;
        thread_data[i].cols = input_matrix[0].size();
        thread_data[i].width = input_matrix[0].size();
        thread_data[i].matrix = &input_matrix;
        thread_data[i].result_matrix = &output_matrix;

        pthread_create(&threads[i], NULL, matrix_erosion, &thread_data[i]);

        current_row = thread_data[i].end_row + 1;
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
}
void multiThreadDilatation(const std::vector<std::vector<int>>& input_matrix, std::vector<std::vector<int>>& output_matrix, int num_threads) {
    int matrix_size = input_matrix.size();
    int rows_per_thread = matrix_size / num_threads;
    int remainder_rows = matrix_size % num_threads;

    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData> thread_data(num_threads);

    int current_row = 0;

    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].start_row = current_row;
        thread_data[i].end_row = current_row + rows_per_thread - 1;
        if (i < remainder_rows) {
            thread_data[i].end_row += 1;
        }
        thread_data[i].rows = matrix_size;
        thread_data[i].cols = input_matrix[0].size();
        thread_data[i].width = input_matrix[0].size();
        thread_data[i].matrix = &input_matrix;
        thread_data[i].result_matrix = &output_matrix;

        pthread_create(&threads[i], NULL, matrix_dilatation, &thread_data[i]);

        current_row = thread_data[i].end_row + 1;
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
}

TEST(MatrixErosionTest, test_1) {
    std::vector<std::vector<int>> input_matrix = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 0, 0},
        {1, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    std::vector<std::vector<int>> expected_output = {
        {1, 1, 0, 0, 0},
        {1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    std::vector<std::vector<int>> output_matrix(input_matrix.size(), std::vector<int>(input_matrix[0].size()));

    singleThreadErosion(input_matrix, output_matrix);

    EXPECT_EQ(output_matrix, expected_output);
}

TEST(MatrixDilatationTest, test_dop) {
    std::vector<std::vector<int>> input_matrix = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 0, 0},
        {1, 1, 1, 0, 0},
        {1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    std::vector<std::vector<int>> expected_output = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 0},
        {1, 1, 1, 1, 0},
        {1, 1, 0, 0, 0}
    };

    std::vector<std::vector<int>> output_matrix(input_matrix.size(), std::vector<int>(input_matrix[0].size()));

    singleThreadDilatation(input_matrix, output_matrix);

    EXPECT_EQ(output_matrix, expected_output);
}

TEST(MatrixErosionTest, test_2) {
    const int size = 100;
    std::vector<std::vector<int>> input_matrix(size, std::vector<int>(size));

    srand(static_cast<unsigned int>(time(NULL)));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            input_matrix[i][j] = rand() % 2;
        }
    }

    std::vector<std::vector<int>> single_thread_output(size, std::vector<int>(size));
    std::vector<std::vector<int>> multi_thread_output(size, std::vector<int>(size));

    singleThreadErosion(input_matrix, single_thread_output);

    for (int num_threads = 2; num_threads <= 8; num_threads *= 2) {
        multiThreadErosion(input_matrix, multi_thread_output, num_threads);

        EXPECT_EQ(single_thread_output, multi_thread_output);
    }
}

TEST(MatrixErosionTest, test_3) {
    const int size = 1000;
    std::vector<std::vector<int>> input_matrix(size, std::vector<int>(size));

    srand(static_cast<unsigned int>(time(NULL)));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            input_matrix[i][j] = rand() % 2;
        }
    }

    std::vector<std::vector<int>> single_thread_output(size, std::vector<int>(size));
    std::vector<std::vector<int>> multi_thread_output(size, std::vector<int>(size));

    auto start_single = std::chrono::high_resolution_clock::now();
    singleThreadErosion(input_matrix, single_thread_output);
    auto end_single = std::chrono::high_resolution_clock::now();
    auto duration_single = std::chrono::duration_cast<std::chrono::milliseconds>(end_single - start_single).count();

    int num_threads = 4;
    auto start_multi = std::chrono::high_resolution_clock::now();
    multiThreadErosion(input_matrix, multi_thread_output, num_threads);
    auto end_multi = std::chrono::high_resolution_clock::now();
    auto duration_multi = std::chrono::duration_cast<std::chrono::milliseconds>(end_multi - start_multi).count();

    EXPECT_EQ(single_thread_output, multi_thread_output);

    EXPECT_LT(duration_multi, duration_single);

    std::cout << "Время однопоточного выполнения: " << duration_single << " мс" << std::endl;
    std::cout << "Время многопоточного выполнения: " << duration_multi << " мс" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
