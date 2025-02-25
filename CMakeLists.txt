cmake_minimum_required(VERSION 3.16)
project(ISA_Simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Qt
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Add source directories
add_subdirectory(src)
add_subdirectory(tests)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
