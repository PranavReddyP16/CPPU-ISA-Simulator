# ────────────────────────────────────────────────────────────────────────────────
#  src/assembler/CMakeLists.txt  –  builds libassembler.a
# ────────────────────────────────────────────────────────────────────────────────

set(ASSEMBLER_SRCS
    assembler.cpp
)

add_library(assembler STATIC ${ASSEMBLER_SRCS})

target_include_directories(assembler
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)



target_link_libraries(assembler
    PUBLIC
        memory          # needs Memory class to write the program in
)
