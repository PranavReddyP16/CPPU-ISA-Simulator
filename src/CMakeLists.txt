set(SOURCES
    isa/memory.cpp
)

# Define executable
add_executable(ISA_Simulator ${SOURCES})

# Link with Qt
target_link_libraries(ISA_Simulator Qt6::Widgets)
