cmake_minimum_required(VERSION 3.10)
project(Lab4DynamicLibraries)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Включаем позиционно-независимый код для создания динамических библиотек
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Добавляем флаг для генерации отладочной информации (опционально)
#set(CMAKE_BUILD_TYPE Debug)

# Добавляем все исходные файлы в переменную SOURCES
set(SOURCES
    prime_naive.cpp
    prime_sieve.cpp
    e_formula.cpp
    e_series.cpp
    test_static.cpp
    test_dynamic.cpp
    unit_tests.cpp
)

# Создаём динамические библиотеки
add_library(prime_naive SHARED prime_naive.cpp)
add_library(prime_sieve SHARED prime_sieve.cpp)
add_library(e_formula SHARED e_formula.cpp)
add_library(e_series SHARED e_series.cpp)

# Устанавливаем имена выходных файлов библиотек без префикса "lib"
set_target_properties(prime_naive PROPERTIES PREFIX "")
set_target_properties(prime_sieve PROPERTIES PREFIX "")
set_target_properties(e_formula PROPERTIES PREFIX "")
set_target_properties(e_series PROPERTIES PREFIX "")

# Добавляем исполняемые файлы
add_executable(test_static test_static.cpp)
add_executable(test_dynamic test_dynamic.cpp)

# Связываем библиотеки с исполняемым файлом test_static
target_link_libraries(test_static
    PRIVATE
        prime_naive
        e_formula
)

# Для test_dynamic необходимо добавить библиотеку dl (для Unix-подобных систем)
if(UNIX)
    target_link_libraries(test_dynamic PRIVATE dl)
endif()

# Настройка Google Test
# Добавляем FetchContent для загрузки Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

# Загружаем и создаём Google Test
FetchContent_MakeAvailable(googletest)

# Включаем тестирование
enable_testing()

# Создаём тестовый исполняемый файл
add_executable(unit_tests unit_tests.cpp)

# Связываем тесты с библиотеками и Google Test
target_link_libraries(unit_tests
    PRIVATE
        gtest_main
        prime_naive
        prime_sieve
        e_formula
        e_series
)

# Добавляем тесты в CTest
include(GoogleTest)
gtest_discover_tests(unit_tests)
