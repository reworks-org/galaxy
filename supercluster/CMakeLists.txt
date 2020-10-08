project(supercluster CXX)

file(GLOB_RECURSE supercluster_src
    "src/*.c"
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
    "src/*.cc"
    "src/*.hh"
    "src/*.inl"
)

source_group(${PROJECT_NAME} ${supercluster_src})
add_executable(${PROJECT_NAME} ${supercluster_src})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
    "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
    "NDEBUG"
    ">"
    "AL_LIBTYPE_STATIC"
    "GLM_FORCE_XYZW_ONLY"
    "GLFW_INCLUDE_NONE"
    "STB_IMAGE_WRITE_STATIC"
    "STB_IMAGE_WRITE_IMPLEMENTATION"
    "STBI_ONLY_PNG"
    "STB_IMAGE_STATIC"
    "STB_IMAGE_IMPLEMENTATION"
    "SOL_CXX17_FEATURES"
    "UNICODE"
    "_UNICODE"
)

if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
        "_CRT_SECURE_NO_WARNINGS"
        "_CRT_SECURE_NO_DEPRECATE"
        "_CRT_NONSTDC_NO_DEPRECATE"
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/supercluster/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/supercluster/bin"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/supercluster/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/supercluster/bin"
)

include(${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME})

if (MSVC)
    if (NOT (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND "x${CMAKE_CXX_SIMULATE_ID}" STREQUAL "xMSVC"))
        target_compile_options(${PROJECT_NAME} PRIVATE
            /QIntel-jcc-erratum
        )
    endif()

    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /JMC
            /ZI
            /Od
            /RTC1
            /MTd
            /GS
            /GR
        >
        $<$<CONFIG:Release>:
            /O2
            /Oi
            /Ot
            /GL
            /MT
            /GS-
            /Gy
            /GR-
        >
        /std:c++latest
        /sdl
        /WX-
        /MP
        /GF
        /EHsc
        /fp:except
        /fp:precise
        /bigobj
        /Zc:preprocessor
        /Y-
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
        /INCREMENTAL
        /DEBUG
        /WX:NO
        >
        $<$<CONFIG:Release>:
        /INCREMENTAL:NO
        /OPT:REF
        /OPT:ICF
        /LTCG:incremental
        /RELEASE
        >
        /ignore:4099
        /MACHINE:X64
        /SUBSYSTEM:CONSOLE
        /verbose:lib
    )
elseif (GNU)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
        -fanalyzer
        >
        $<$<CONFIG:Release>:
        >
        -std=c++20
        -pthread
        -fexceptions
    )

    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
        -Og
        >
        $<$<CONFIG:Release>:
        -O3
        >
        -pthread
        -static-libstdc++
        -static-libgcc
        -static
    )
endif()

# Postbuild shader copy.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_custom_command(TARGET supercluster POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/galaxy/quasar/shaders" "${CMAKE_SOURCE_DIR}/supercluster/bin/Debug/assets/shaders")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_custom_command(TARGET supercluster POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/galaxy/quasar/shaders" "${CMAKE_SOURCE_DIR}/supercluster/bin/Release/assets/shaders")
else()
    message(FATAL_ERROR "Could not determine build configuration. Is currently: ${CMAKE_BUILD_TYPE}")
endif()
