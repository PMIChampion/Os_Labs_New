// tests/tests.cpp

#include <gtest/gtest.h>
#include <string>
#include <array>
#include <cstdio>
#include <memory>
#include <stdexcept>

// Функция для выполнения команды и получения результата
std::string executeCommand(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    // Открываем поток для чтения вывода команды
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    // Читаем вывод команды
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// Макрос для упрощения написания тестов
#define RUN_TEST(input, expected_output) \
    do { \
        std::string cmd = "echo \"" input "\" | " "./lab-1"; \
        std::string output; \
        try { \
            output = executeCommand(cmd); \
        } catch (const std::exception& e) { \
            FAIL() << "Failed to execute command: " << e.what(); \
        } \
        EXPECT_NE(output.find(expected_output), std::string::npos) << "Expected \"" << expected_output << "\" not found in output."; \
    } while(0)

// Тестовый случай 1: корректные входные данные
TEST(Lab1Tests, CorrectInput) {
    RUN_TEST("10 2 5\n", "Результат: 1");
}

// Тестовый случай 2: деление на ноль
TEST(Lab1Tests, DivisionByZero) {
    RUN_TEST("10 0 5\n", "Делить на ноль нельзя!");
}

// Тестовый случай 3: отрицательные числа
TEST(Lab1Tests, NegativeNumbers) {
    RUN_TEST("-10 -2 5\n", "Результат: 1");
}

// Тестовый случай 4: большие числа
TEST(Lab1Tests, LargeNumbers) {
    RUN_TEST("1000 10 10\n", "Результат: 10");
}

// Тестовый случай 5: оба делителя ноль
TEST(Lab1Tests, BothDivisorsZero) {
    RUN_TEST("10 0 0\n", "Делить на ноль нельзя!");
}

// Тестовый случай 6: некорректный ввод
TEST(Lab1Tests, InvalidInput) {
    RUN_TEST("10 a 5\n", "Некорректный ввод"); // Предполагается, что программа выведет "Ошибка" или аналогичное сообщение
}
