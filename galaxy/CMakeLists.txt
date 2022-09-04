project(galaxy CXX)

file(GLOB_RECURSE galaxy_src
    "src/*.cpp"
    "src/*.hpp"
)

source_group(${PROJECT_NAME} ${galaxy_src})
add_library(${PROJECT_NAME} STATIC ${galaxy_src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
)