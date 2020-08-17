project(frb CXX)

set(frb_src
    "src/frb/audio/Music.cpp"
    "src/frb/audio/Sound.cpp"
    "src/frb/buffer/Buffer.cpp"
    "src/frb/buffer/BufferStream.cpp"
    "src/frb/Context.cpp"
    "src/frb/detail/Error.cpp"
    "src/frb/detail/Source.cpp"
    "src/frb/detail/SourceManipulator.cpp"
    "src/frb/audio/Music.hpp"
    "src/frb/audio/Sound.hpp"
    "src/frb/buffer/Buffer.hpp"
    "src/frb/buffer/BufferStream.hpp"
    "src/frb/Context.hpp"
    "src/frb/detail/Error.hpp"
    "src/frb/detail/Source.hpp"
    "src/frb/detail/SourceManipulator.hpp"
)

source_group(${PROJECT_NAME} ${frb_src})
add_library(${PROJECT_NAME} STATIC ${frb_src})

target_include_directories(${PROJECT_NAME} PUBLIC
    "src/"
    "../../dependencies/oggvorbis/include/"
    "../../dependencies/openal/include/"
    "../../dependencies/stb/"
    "../../dependencies/c++20/"
    "../pulsar/src/"
    "../protostar/src/"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
    "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
    "NDEBUG"
    ">"
    "AL_LIBTYPE_STATIC"
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
endif()
