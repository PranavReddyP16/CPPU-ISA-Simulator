cmake_minimum_required(VERSION 3.16)
project(ISA_Simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
