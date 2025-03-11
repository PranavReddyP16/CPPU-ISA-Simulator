# Define sources for the ISA core library
set(SOURCES
    memory/memory.cpp
    memory/cache.cpp
    memory/cache_line.cpp
    memory/cache_set.cpp
)

# Create a library for ISA core functionality
add_library(isa_core ${SOURCES})

# Ensure headers in `include/` and `src/` are visible to dependent targets
target_include_directories(isa_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Define the main ISA simulator executable
add_executable(ISA_Simulator main.cpp)

# Link the executable with the ISA core library
target_link_libraries(ISA_Simulator PRIVATE isa_core)

# If Qt6 is needed, keep this line; otherwise, remove it.
target_link_libraries(ISA_Simulator PRIVATE Qt6::Widgets)
