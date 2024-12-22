cmake_minimum_required(VERSION 3.10)
project(Battleship)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)

# Добавляем каталоги для поиска заголовочных файлов
include_directories(/opt/homebrew/include)
include_directories(${CMAKE_SOURCE_DIR})  # Добавляем текущую директорию для заголовков

# Добавляем каталоги для поиска библиотек
link_directories(/opt/homebrew/lib)

# Создаём библиотеку для общего кода
# Предполагается, что Board.cpp и GameRoom.cpp находятся в корне проекта
add_library(battleship_lib
    Board.cpp
    GameRoom.cpp
)

# Добавляем исполняемый файл сервера и связываем его с ZeroMQ и библиотекой
add_executable(server server.cpp)
target_link_libraries(server PRIVATE zmq battleship_lib)

# Добавляем исполняемый файл клиента и связываем его с ZeroMQ и библиотекой
add_executable(client client.cpp)
target_link_libraries(client PRIVATE zmq battleship_lib)

# Добавляем Google Test
enable_testing()

# Находим установленный Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Создаём исполняемый файл для тестов и связываем его с Google Test и библиотекой
add_executable(tests tests/tests.cpp)

# Линкуем тесты с Google Test и общей библиотекой
target_link_libraries(tests PRIVATE GTest::gtest_main battleship_lib zmq)

# Подключаем Google Test к CTest
include(GoogleTest)
gtest_discover_tests(tests)




