cmake_minimum_required(VERSION 3.18)
project(led_cube)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(depedencies/json)

find_library(GPIOD_LIBRARY gpiodcxx
        REQUIRED)

add_executable(led_cube main.cpp)
target_link_libraries(led_cube ${GPIOD_LIBRARY} nlohmann_json::nlohmann_json)

# g++ -c main.cpp -o main.o
# g++ -o main main.o -lgpiodcxx
