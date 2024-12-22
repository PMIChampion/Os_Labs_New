#include "../include/my_solution.h"


const int KERNEL_SIZE = 3;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
std::vector<std::vector<int>> filter = {
    {1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}
};



// Эрозия матрицы
void* matrix_erosion(void* arg){

    ThreadData* data = static_cast<ThreadData*>(arg);
    const std::vector<std::vector<int>>& matrix = *(data -> matrix);
    std::vector<std::vector<int>>& result_matrix = *(data -> result_matrix);
    int m_rows = data -> rows;
    int m_cols = data -> cols;
    std::cout << "Поток обрабатывает строки с " << data->start_row 
              << " по " << data->end_row << std::endl;
    for (int i = data -> start_row; i <= data -> end_row; ++i){
        for (int j = 0; j < m_cols; ++j){
            bool flag = false;

            for (int ki = -KERNEL_SIZE / 2; ki <= KERNEL_SIZE / 2; ++ki){
                for (int kj = -KERNEL_SIZE / 2; kj <= KERNEL_SIZE / 2; ++kj){
                    int ni = i + ki;
                    int nj = j + kj;
                    if (ni >= 0 && nj >= 0 && ni < m_rows && nj < m_cols){
                        if (filter[ki + KERNEL_SIZE / 2][kj + KERNEL_SIZE / 2] != matrix[ni][nj]){
                            flag = true;
                        }
                    }
                }
            }
            
            pthread_mutex_lock(&mtx);
            if (flag == true){
                result_matrix[i][j] = 0;
            }else{
                result_matrix[i][j] = 1;
            }
            pthread_mutex_unlock(&mtx);
        }
    }
    return nullptr;
}

// Наращивание матрицы
void* matrix_dilatation(void* arg){

    ThreadData* data = static_cast<ThreadData*>(arg);
    const std::vector<std::vector<int>>& matrix = *(data -> matrix);
    std::vector<std::vector<int>>& result_matrix = *(data -> result_matrix);
    int m_rows = data -> rows;
    int m_cols = data -> cols;

    for (int i = data -> start_row; i <= data -> end_row; ++i){
        for (int j = 0; j < m_cols; ++j){
            bool flag = false;

            for (int ki = -KERNEL_SIZE / 2; ki <= KERNEL_SIZE / 2; ++ki){
                for (int kj = -KERNEL_SIZE / 2; kj <= KERNEL_SIZE / 2; ++kj){
                    int ni = i + ki;
                    int nj = j + kj;
                    if (ni >= 0 && nj >= 0 && ni < m_rows && nj < m_cols){
                        if ((filter[ki + KERNEL_SIZE / 2][kj + KERNEL_SIZE / 2] == 1) && (matrix[ni][nj] == 1)){
                            flag = true;
                            break;
                            
                        }
                    }
                }
                if (flag) break;
            }
            
            // pthread_mutex_lock(&mtx);
            if (flag == true){
                result_matrix[i][j] = 1;
            }else{
                result_matrix[i][j] = 0;
            }
            // pthread_mutex_unlock(&mtx);
        }
    }
    return nullptr;
}


