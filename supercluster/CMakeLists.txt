project(supercluster CXX)

set(supercluster_src
"src/layers/ECSLayer.cpp"
"src/layers/ECSLayer.hpp"
"src/layers/GUILayer.cpp"
"src/layers/GUILayer.hpp"
"src/layers/ImGuiLayer.cpp"
"src/layers/ImGuiLayer.hpp"
"src/main.cpp"
"src/states/Editor.cpp"
"src/states/Editor.hpp"
)

source_group(${PROJECT_NAME} ${supercluster_src})
add_executable(${PROJECT_NAME} ${supercluster_src})

target_include_directories(${PROJECT_NAME} PUBLIC
"../galaxy/galaxy/src/"
"../galaxy/deps/src/"
"../galaxy/frb/src/"
"../galaxy/protostar/src/"
"../galaxy/pulsar/src/"
"../galaxy/quasar/src/"
"../galaxy/redshift/src/"
"../galaxy/solar/src/"
"../galaxy/starlight/src/"
"../galaxy/starmap/src/"
"../dependencies/freetype/include/"
"../dependencies/glad/"
"../dependencies/glfw3/include/"
"../dependencies/glm/"
"../dependencies/imgui/"
"../dependencies/imgui/cpp/"
"../dependencies/imgui/impl/"
"../dependencies/imgui-texteditor/"
"../dependencies/json/include/"
"../dependencies/lua/src/"
"../dependencies/openal/include/"
"../dependencies/portable-file-dialogs/include/"
"../dependencies/sol3/single/include/"
"../dependencies/stb/"
"../dependencies/zlib/include/"
"../dependencies/googletest/googletest/include/"
"../dependencies/googletest/googletest/"
"../../dependencies/Box2D-MT/"
"../../dependencies/c++20/"
)

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

target_link_directories(${PROJECT_NAME} PUBLIC
"../dependencies/freetype/lib/$<CONFIG>/"
"../dependencies/glfw3/lib/$<CONFIG>/"
"../dependencies/oggvorbis/lib/$<CONFIG>/"
"../dependencies/openal/lib/$<CONFIG>/"
"../dependencies/zlib/lib/$<CONFIG>/"
"${CMAKE_SOURCE_DIR}/output/bin/frb/$<CONFIG>/"
"${CMAKE_SOURCE_DIR}/output/bin/galaxy/$<CONFIG>/"
"${CMAKE_SOURCE_DIR}/output/bin/protostar/$<CONFIG>/"
"${CMAKE_SOURCE_DIR}/output/bin/pulsar/$<CONFIG>/"
"${CMAKE_SOURCE_DIR}/output/bin/quasar/$<CONFIG>/"
"${CMAKE_SOURCE_DIR}/output/bin/redshift/$<CONFIG>/"
"${CMAKE_SOURCE_DIR}/output/bin/solar/$<CONFIG>/"
"${CMAKE_SOURCE_DIR}/output/bin/starlight/$<CONFIG>/"
"${CMAKE_SOURCE_DIR}/output/bin/starmap/$<CONFIG>/"
"${CMAKE_SOURCE_DIR}/output/bin/deps/$<CONFIG>/"
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

)

target_link_options(${PROJECT_NAME} PRIVATE
)

endif()
