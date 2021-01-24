project(sandbox_test CXX)

file(GLOB_RECURSE sandbox_test_src
    "src/*.cpp"
    "src/*.hpp"
)

source_group(${PROJECT_NAME} ${sandbox_test_src})
add_executable(${PROJECT_NAME} ${sandbox_test_src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
)

# Postbuild OpenAL DLL copy.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/${CMAKE_BUILD_TYPE}/OpenAL32.dll" "${CMAKE_SOURCE_DIR}/sandbox_tests/bin/${CMAKE_BUILD_TYPE}/OpenAL32.dll")
