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
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "_GLIBCXX_USE_NANOSLEEP"
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sandbox_tests/bin"
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
        /experimental:external
        /external:anglebrackets
        /external:W0
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
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
        -fanalyzer
        >
        $<$<CONFIG:Release>:
        >
        -std=c++2a
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
    )
endif()

# Postbuild shader copy.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_custom_command(TARGET sandbox_test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/galaxy/quasar/shaders" "${CMAKE_SOURCE_DIR}/sandbox_tests/bin/Debug/assets/shaders")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_custom_command(TARGET sandbox_test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/galaxy/quasar/shaders" "${CMAKE_SOURCE_DIR}/sandbox_tests/bin/Debug/assets/shaders")
else()
    message(FATAL_ERROR "Could not determine build configuration. Is currently: ${CMAKE_BUILD_TYPE}")
endif()
