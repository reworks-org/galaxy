project(dependencies C CXX)

file(GLOB_RECURSE dependencies_src
    "${CMAKE_SOURCE_DIR}/dependencies/cpp20/date/src/tz.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/cpp20/os.cc"
    "${CMAKE_SOURCE_DIR}/dependencies/cpp20/format.cc"
    "${CMAKE_SOURCE_DIR}/dependencies/glad/glad/glad.c"
    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googlemock/src/gmock-all.cc"
    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest/gtest-all.cc"
    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest/gtest-internal-inl.h"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/lua/src/*.c"
)

source_group(${PROJECT_NAME} ${dependencies_src})
add_library(${PROJECT_NAME} STATIC ${dependencies_src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
)