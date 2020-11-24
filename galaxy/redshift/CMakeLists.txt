project(redshift CXX)

file(GLOB_RECURSE redshift_src
    "src/*.c"
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
    "src/*.cc"
    "src/*.hh"
    "src/*.inl"
)

source_group(${PROJECT_NAME} ${redshift_src})
add_library(${PROJECT_NAME} STATIC ${redshift_src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
)