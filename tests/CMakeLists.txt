project(tests CXX)

file(GLOB_RECURSE tests_src
    "src/*.c"
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
    "src/*.cc"
    "src/*.hh"
    "src/*.inl"
)

source_group(${PROJECT_NAME} ${tests_src})
add_executable(${PROJECT_NAME} ${tests_src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/bin"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/bin"
)