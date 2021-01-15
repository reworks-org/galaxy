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

# Postbuild shader copy.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_custom_command(TARGET supercluster POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/galaxy/src/galaxy/graphics/shaders" "${CMAKE_SOURCE_DIR}/supercluster/bin/Debug/assets/shaders")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_custom_command(TARGET supercluster POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/galaxy/src/galaxy/graphics/shaders" "${CMAKE_SOURCE_DIR}/supercluster/bin/Debug/assets/shaders")
else()
    message(FATAL_ERROR "Could not determine build configuration. Is currently: ${CMAKE_BUILD_TYPE}")
endif()