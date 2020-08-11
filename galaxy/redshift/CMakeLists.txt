project(redshift CXX)

set(redshift_src
"src/redshift/dummy.cpp"
)

source_group(${PROJECT_NAME} ${redshift_src})
add_library(${PROJECT_NAME} STATIC ${redshift_src})

target_include_directories(${PROJECT_NAME} PUBLIC
"src/"
"../../dependencies/c++20/"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
"$<$<CONFIG:Debug>:"
"_DEBUG"
">"
"$<$<CONFIG:Release>:"
"NDEBUG"
">"
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

if(MSVC)
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
/QIntel-jcc-erratum
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

else()

target_compile_options(${PROJECT_NAME} PRIVATE
$<$<CONFIG:Debug>:
-fcheck-new
-fanalyzer
>
$<$<CONFIG:Release>:
>
-std=c++20
-funsigned-char
-fchar8_t
-fconcepts
-fcoroutines
-fno-pretty-templates
-fstrict-enums
-pthread
-fexceptions
)

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
