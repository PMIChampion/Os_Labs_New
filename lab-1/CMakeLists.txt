cmake_minimum_required(VERSION 3.10)
project(LabProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавление директории с заголовочными файлами
include_directories(include)

# Компиляция исполняемого файла для child
add_executable(child src/child.cpp)
target_include_directories(child PRIVATE include)

# Компиляция основного исполняемого файла lab-1
add_executable(lab-1 main.cpp src/parent.cpp src/utils.cpp)
target_include_directories(lab-1 PRIVATE include)

# Обеспечиваем, что child собирается перед lab-1
add_dependencies(lab-1 child)

# Включение тестирования
enable_testing()

# Поиск Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Компиляция тестового исполняемого файла
add_executable(run_tests tests/tests.cpp)
target_include_directories(run_tests PRIVATE include)

# Линковка с Google Test и pthread
target_link_libraries(run_tests PRIVATE GTest::gtest_main pthread)

# Добавление тестов к CTest
add_test(NAME Lab1Tests COMMAND run_tests)
