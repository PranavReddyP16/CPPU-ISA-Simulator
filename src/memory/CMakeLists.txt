# src/memory/CMakeLists.txt – builds libmemory.a

set(MEMORY_SRCS
    Memory.cpp
    Register.cpp
    Cache.cpp
    cache_set.cpp
    cache_line.cpp
    utils.cpp         # ← added so mem_dtype_to_int, int_to_mem_dtype, print_data, etc. get compiled
)

set(CMAKE_AUTOMOC ON)

add_library(memory STATIC ${MEMORY_SRCS})

target_include_directories(memory
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(memory
    PUBLIC
        Qt6::Core
)

# No need to link Qt here; backend is GUI-free
