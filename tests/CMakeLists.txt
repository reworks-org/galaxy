project(tests CXX)

file(GLOB_RECURSE tests_src
    "src/*.cpp"
    "src/*.hpp"
)

source_group(${PROJECT_NAME} ${tests_src})
add_executable(${PROJECT_NAME} ${tests_src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/bin"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/bin"
)