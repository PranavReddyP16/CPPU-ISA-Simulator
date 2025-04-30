# ────────────────────────────────────────────────────────────────────────────────
#  src/memory/CMakeLists.txt  –  builds libmemory.a
# ────────────────────────────────────────────────────────────────────────────────

set(MEMORY_SRCS
    Memory.cpp
    Register.cpp
    Cache.cpp
    cache_set.cpp
    cache_line.cpp
)

add_library(memory STATIC ${MEMORY_SRCS})

target_include_directories(memory
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}     # src/memory/
        ${CMAKE_SOURCE_DIR}/include     # top-level include/
        ${CMAKE_SOURCE_DIR}/src         # gives access to gui/mainwindow.h
)

target_link_libraries(memory
    PUBLIC
)
