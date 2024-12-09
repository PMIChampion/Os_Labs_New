#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include "child.hpp"

int calculation(int a, int b, int c){
    return a / b / c;
}

int main(){
    std::string input;

    std::getline(std::cin, input);
    std::stringstream ss(input);
    std::string result;
    bool flag = true;

    int num1, num2, num3;

    if (ss >> num1 >> num2 >> num3) {
        std::string extra;

        if (ss >> extra){
            result = "Некорректный ввод";
            flag = false;
        }
    } else{
        flag = false;
        result = "Некорректный ввод";
    }

    if (flag == true){
        if ((num2 == 0) || (num3 == 0)){
        result = "Делить на ноль нельзя!";
    }   else{

        result = "Результат: " + std::to_string(calculation(num1, num2, num3));
        }
    }

    std::ofstream outfile("result.txt");
    if (!outfile) {
        std::cerr << "Ошибка при открытии файла для записи" << std::endl;
        return 1;
    }
    outfile << result << std::endl;
    outfile.close();

    if (write(STDOUT_FILENO, result.c_str(), result.size()) == -1){
        perror("Не получается отдать результат родителю");
        return 1;
    }
    return 0;


}