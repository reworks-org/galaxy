project(sandbox_test CXX)

file(GLOB_RECURSE sandbox_test_src
    "src/*.c"
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
    "src/*.cc"
    "src/*.hh"
    "src/*.inl"
)

source_group(${PROJECT_NAME} ${sandbox_test_src})
add_executable(${PROJECT_NAME} ${sandbox_test_src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
)

# Postbuild shader copy.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_custom_command(TARGET sandbox_test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/galaxy/quasar/shaders" "${CMAKE_SOURCE_DIR}/sandbox_tests/bin/Debug/assets/shaders")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_custom_command(TARGET sandbox_test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/galaxy/quasar/shaders" "${CMAKE_SOURCE_DIR}/sandbox_tests/bin/Debug/assets/shaders")
else()
    message(FATAL_ERROR "Could not determine build configuration. Is currently: ${CMAKE_BUILD_TYPE}")
endif()