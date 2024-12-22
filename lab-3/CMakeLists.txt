cmake_minimum_required(VERSION 3.28)
project(lab1)

set(CMAKE_CXX_STANDARD 17)

# Подключение GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  TLS_VERIFY false
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Исполняемый файл для родительского процесса
add_executable(lab1 main.cpp src/parent.cpp src/utils.cpp)
target_include_directories(lab1 PRIVATE include)

# Исполняемый файл для дочернего процесса
add_executable(child src/child.cpp src/utils.cpp)
target_include_directories(child PRIVATE include)

# Настройка тестов
enable_testing()
add_executable(tests tests/tests.cpp src/parent.cpp src/utils.cpp)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE GTest::gtest_main pthread)

# Настройка GoogleTest для автоматического обнаружения тестов
include(GoogleTest)
gtest_discover_tests(tests)
