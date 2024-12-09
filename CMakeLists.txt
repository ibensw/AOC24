project(AOC24)
cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt REQUIRED)

add_executable(d1p1 day1/part1.cpp)
target_link_libraries(d1p1 PRIVATE fmt::fmt)

add_executable(d1p2 day1/part2.cpp)
target_link_libraries(d1p2 PRIVATE fmt::fmt)
