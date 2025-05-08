# ────────────────────────────────────────────────────────────────────────────────
#  src/CMakeLists.txt  —  builds libraries and the 'simulator' executable
# ────────────────────────────────────────────────────────────────────────────────

# ----------------------------------------------------------------------
#  Sub-libraries
# ----------------------------------------------------------------------
add_subdirectory(memory)      # src/memory/
add_subdirectory(assembler)   # src/assembler/
add_subdirectory(pipeline)    # src/pipeline/

# ----------------------------------------------------------------------
#  Source files
# ----------------------------------------------------------------------
set(GUI_SRCS
    gui/mainwindow.cpp
)

set(CORE_SRCS
    main.cpp
    # add other core .cpp files here if they’re not part of a sub-library
)

# ----------------------------------------------------------------------
#  Executable
# ----------------------------------------------------------------------
add_executable(simulator
    ${CORE_SRCS}
    ${GUI_SRCS}
)

target_include_directories(simulator
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}          # src/
)

target_link_libraries(simulator
    PRIVATE
        Qt6::Widgets
        Qt6::Concurrent
        memory          # ← supplies Register, Memory, Cache
        assembler
        pipeline
)

set_target_properties(simulator PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)
