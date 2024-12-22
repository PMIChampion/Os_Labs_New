cmake_minimum_required(VERSION 3.10)
project(lab-2)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем директорию include/
include_directories(include)

# Добавляем исходные файлы
set(SOURCES
    src/my_solution.cpp
    main.cpp
)

# Основной исполняемый файл
add_executable(main ${SOURCES})
target_link_libraries(main pthread)

# Подключаем Google Test через FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# Предотвращаем переопределение настроек компилятора/линкера родительского проекта
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Добавляем тесты
add_executable(run_tests tests/test.cpp src/my_solution.cpp)
target_link_libraries(run_tests gtest pthread)

# Добавляем пути к заголовочным файлам Google Test
target_include_directories(run_tests PRIVATE
    ${gtest_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}
)
