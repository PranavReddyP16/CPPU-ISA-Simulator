cmake_minimum_required(VERSION 3.16)
project(pipeline)

# Pipeline stages + instruction helpers
set(PIPELINE_SRCS
    five_stage_pipeline.cpp
    instruction_helpers.cpp
    # point to the ALU implementation one level up:
    ../alu/ALU.cpp
)

# Build a static lib of the pipeline
add_library(pipeline STATIC ${PIPELINE_SRCS})

# Make headers in this folder and the parent 'alu' folder visible
target_include_directories(pipeline PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Link against your memory subsystem
target_link_libraries(pipeline PUBLIC memory)
