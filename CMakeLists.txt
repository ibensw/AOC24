cmake_minimum_required(VERSION 3.25)

project(AOC24 VERSION 0.0.0)
include_directories(util)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt REQUIRED)

add_executable(d1p1 day1/part1.cpp)
target_link_libraries(d1p1 PRIVATE fmt::fmt)

add_executable(d1p2 day1/part2.cpp)
target_link_libraries(d1p2 PRIVATE fmt::fmt)

add_executable(d2p1 day2/part1.cpp)
target_link_libraries(d2p1 PRIVATE fmt::fmt)

add_executable(d2p2 day2/part2.cpp)
target_link_libraries(d2p2 PRIVATE fmt::fmt)

include(FetchContent)
FetchContent_Declare(
  ctre
  GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
  GIT_TAG v3.7  # Replace with the desired version
)
FetchContent_MakeAvailable(ctre)

add_executable(d3p1 day3/part1.cpp)
target_link_libraries(d3p1 PRIVATE ctre::ctre)
target_link_libraries(d3p1 PRIVATE fmt::fmt)

add_executable(d3p2 day3/part2.cpp)
target_link_libraries(d3p2 PRIVATE ctre::ctre)
target_link_libraries(d3p2 PRIVATE fmt::fmt)

add_executable(d4p1 day4/part1.cpp)
target_link_libraries(d4p1 PRIVATE fmt::fmt)

add_executable(d4p2 day4/part2.cpp)
target_link_libraries(d4p2 PRIVATE fmt::fmt)

add_executable(d5p1 day5/part1.cpp)
target_link_libraries(d5p1 PRIVATE ctre::ctre)
target_link_libraries(d5p1 PRIVATE fmt::fmt)

add_executable(d5p2 day5/part2.cpp)
target_link_libraries(d5p2 PRIVATE ctre::ctre)
target_link_libraries(d5p2 PRIVATE fmt::fmt)

add_executable(d6p1 day6/part1.cpp)
target_link_libraries(d6p1 PRIVATE fmt::fmt)

add_executable(d6p2 day6/part2.cpp)
target_link_libraries(d6p2 PRIVATE fmt::fmt)

add_executable(d7p1 day7/part1.cpp)
target_link_libraries(d7p1 PRIVATE ctre::ctre)
target_link_libraries(d7p1 PRIVATE fmt::fmt)

add_executable(d7p2 day7/part2.cpp)
target_link_libraries(d7p2 PRIVATE ctre::ctre)
target_link_libraries(d7p2 PRIVATE fmt::fmt)

add_executable(d8p1 day8/part1.cpp)
target_link_libraries(d8p1 PRIVATE fmt::fmt)

add_executable(d8p2 day8/part2.cpp)
target_link_libraries(d8p2 PRIVATE fmt::fmt)

add_executable(d9p1 day9/part1.cpp)
target_link_libraries(d9p1 PRIVATE fmt::fmt)

add_executable(d9p2 day9/part2.cpp)
target_link_libraries(d9p2 PRIVATE fmt::fmt)

add_executable(d10p1 day10/part1.cpp)
target_link_libraries(d10p1 PRIVATE fmt::fmt)

add_executable(d10p2 day10/part2.cpp)
target_link_libraries(d10p2 PRIVATE fmt::fmt)

add_executable(d11p1 day11/part1.cpp)
target_link_libraries(d11p1 PRIVATE fmt::fmt)

add_executable(d11p2 day11/part2.cpp)
target_link_libraries(d11p2 PRIVATE fmt::fmt)

add_executable(d12p1 day12/part1.cpp)
target_link_libraries(d12p1 PRIVATE fmt::fmt)

add_executable(d12p2 day12/part2.cpp)
target_link_libraries(d12p2 PRIVATE fmt::fmt)

