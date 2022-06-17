cmake_minimum_required(VERSION 3.22)
project(led_cube)

set(CMAKE_CXX_STANDARD 20)

find_library(our_gpiod gpiodcxx
        HINTS /usr/lib/x86_64-linux-gnu
        REQUIRED)

add_executable(led_cube main.cpp)
target_link_libraries(led_cube ${our_gpiod})

# g++ -c main.cpp -o main.o
# g++ -o main main.o -lgpiodcxx
