project(dependencies C CXX)

file(GLOB_RECURSE dependencies_src
    "${CMAKE_SOURCE_DIR}/dependencies/glad/glad/glad.c"
    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googlemock/src/gmock-all.cc"
    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest/gtest-all.cc"
    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest/gtest-internal-inl.h"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui/addons/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/lua/src/*.c"
    "${CMAKE_SOURCE_DIR}/dependencies/sqlite/src/sqlite3.c"
    "${CMAKE_SOURCE_DIR}/dependencies/stb/stb_vorbis.c"
    "${CMAKE_SOURCE_DIR}/dependencies/zlc/lib/*.cpp"
)

source_group(${PROJECT_NAME} ${dependencies_src})
add_library(${PROJECT_NAME} STATIC ${dependencies_src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
)

# Copy zlib header.
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/dependencies/zlib-ng/zconf.h" "${CMAKE_SOURCE_DIR}/dependencies/zconf/zconf.h")