#include <gtest/gtest.h>
#include "parent.hpp"
#include <sstream>
#include <vector>
#include <numeric>
#include <filesystem>

namespace fs = std::filesystem;

void TestParent(const std::string& input, const std::string& expectedOutput, const std::string& pathToChild) {
    std::stringstream inFile(input);
    std::stringstream outFile;

    if (fs::exists(pathToChild)) {
        ParentProcess(pathToChild.c_str(), inFile, outFile);

        std::string result = outFile.str();

        EXPECT_EQ(result, expectedOutput);
    } else {
        std::cerr << "Путь к дочернему процессу не существует: " << pathToChild << std::endl;
        FAIL() << "Путь к дочернему процессу не существует";
    }
}

const std::string PATH_TO_CHILD = getenv("WAY_TO_FILE"); 
TEST(ParentTest, CorrectCalculation) {
    std::string input = "100 2 5\nexit\n";
    std::string expected_output = "Результат: 10\n";
    TestParent(input, expected_output, PATH_TO_CHILD);
}

TEST(ParentTest, DivisionByZero) {
    std::string input = "10 0 5\nexit\n";
    std::string expected_output = "Деление на ноль\n";
    TestParent(input, expected_output, PATH_TO_CHILD);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
