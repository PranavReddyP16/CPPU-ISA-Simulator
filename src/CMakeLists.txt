# Define sources for the ISA core library
set(SOURCES
    memory/Memory.cpp
    memory/Cache.cpp
)

# Create a library for ISA core functionality
add_library(isa_core ${SOURCES})

# Ensure headers in `src/` are visible to dependent targets
target_include_directories(isa_core PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Define the main ISA simulator executable
add_executable(ISA_Simulator main.cpp)

# Link the executable with the ISA core library
target_link_libraries(ISA_Simulator PRIVATE isa_core)

# If Qt6 is actually needed, keep this line. Otherwise, remove it.
# target_link_libraries(ISA_Simulator PRIVATE Qt6::Widgets)
