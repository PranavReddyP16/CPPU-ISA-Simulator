# ────────────────────────────────────────────────────────────────────────────────
#  src/pipeline/CMakeLists.txt   – builds libpipeline.a
# ────────────────────────────────────────────────────────────────────────────────

set(PIPELINE_SRCS
    five_stage_pipeline.cpp
    instruction_helpers.cpp
)

add_library(pipeline STATIC ${PIPELINE_SRCS})

target_include_directories(pipeline
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)



target_link_libraries(pipeline
    PUBLIC
        memory                  # needs Register, Cache, Memory
)
