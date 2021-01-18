project(supercluster CXX)

file(GLOB_RECURSE supercluster_src
    "src/*.cpp"
    "src/*.hpp"
)

source_group(${PROJECT_NAME} ${supercluster_src})
add_executable(${PROJECT_NAME} ${supercluster_src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/supercluster/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/supercluster/bin"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/supercluster/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/supercluster/bin"
)