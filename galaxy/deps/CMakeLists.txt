project(deps C CXX)

file(GLOB_RECURSE dependencies_src
    "${CMAKE_SOURCE_DIR}/dependencies/Box2D-MT/Box2D/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/c++20/*.cc"
    "${CMAKE_SOURCE_DIR}/dependencies/c++20/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/glad/*.c"
    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest/gtest-all.cc"
    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest/gtest-internal-inl.h"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/imgui-texteditor/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/lua/src/*.c"
)

source_group(${PROJECT_NAME} ${dependencies_src})
add_library(${PROJECT_NAME} STATIC ${dependencies_src})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
    "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
    "NDEBUG"
    ">"
    "IMGUI_IMPL_OPENGL_LOADER_GLAD"
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
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/${PROJECT_NAME}/"
)

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
        /W0
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
        /SUBSYSTEM:CONSOLE
        >
        $<$<CONFIG:Release>:
        /INCREMENTAL:NO
        /SUBSYSTEM:WINDOWS
        /OPT:REF
        /OPT:ICF
        /LTCG:incremental
        >
        /SAFESEH
        /ignore:4099
        /MACHINE:X64
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
