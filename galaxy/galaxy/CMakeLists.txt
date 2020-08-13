project(galaxy CXX)

set(galaxy_src
"src/galaxy/components/All.cpp"
"src/galaxy/components/All.hpp"
"src/galaxy/components/AnimationComponent.cpp"
"src/galaxy/components/AnimationComponent.hpp"
"src/galaxy/components/MusicComponent.cpp"
"src/galaxy/components/MusicComponent.hpp"
"src/galaxy/components/ShaderComponent.cpp"
"src/galaxy/components/ShaderComponent.hpp"
"src/galaxy/components/SoundComponent.cpp"
"src/galaxy/components/SoundComponent.hpp"
"src/galaxy/components/SpriteBatchComponent.cpp"
"src/galaxy/components/SpriteBatchComponent.hpp"
"src/galaxy/components/SpriteComponent.cpp"
"src/galaxy/components/SpriteComponent.hpp"
"src/galaxy/core/Application.cpp"
"src/galaxy/core/Application.hpp"
"src/galaxy/core/ServiceLocator.cpp"
"src/galaxy/core/ServiceLocator.hpp"
"src/galaxy/core/World.cpp"
"src/galaxy/core/World.hpp"
"src/galaxy/flags/EnabledFlag.cpp"
"src/galaxy/flags/EnabledFlag.hpp"
"src/galaxy/fs/Config.cpp"
"src/galaxy/fs/Config.hpp"
"src/galaxy/fs/FileSystem.cpp"
"src/galaxy/fs/FileSystem.hpp"
"src/galaxy/fs/Serializable.cpp"
"src/galaxy/fs/Serializable.hpp"
"src/galaxy/layer/Layer.cpp"
"src/galaxy/layer/Layer.hpp"
"src/galaxy/layer/LayerStorage.cpp"
"src/galaxy/layer/LayerStorage.hpp"
"src/galaxy/res/FontBook.cpp"
"src/galaxy/res/FontBook.hpp"
"src/galaxy/res/ShaderBook.cpp"
"src/galaxy/res/ShaderBook.hpp"
"src/galaxy/scripting/Command.cpp"
"src/galaxy/scripting/Command.hpp"
"src/galaxy/scripting/JSONDefinition.cpp"
"src/galaxy/scripting/JSONDefinition.hpp"
"src/galaxy/scripting/LuaConsole.cpp"
"src/galaxy/scripting/LuaConsole.hpp"
"src/galaxy/scripting/LuaEntity.cpp"
"src/galaxy/scripting/LuaEntity.hpp"
"src/galaxy/scripting/LuaUtils.cpp"
"src/galaxy/scripting/LuaUtils.hpp"
)

source_group(${PROJECT_NAME} ${galaxy_src})
add_library(${PROJECT_NAME} STATIC ${galaxy_src})

target_include_directories(${PROJECT_NAME} PUBLIC
"src/"
"../celestial/src/"
"../deps/src/"
"../frb/src/"
"../protostar/src/"
"../pulsar/src/"
"../quasar/src/"
"../redshift/src/"
"../solar/src/"
"../starlight/src/"
"../starmap/src/"
"../../dependencies/freetype/include/"
"../../dependencies/glad/"
"../../dependencies/glfw3/include/"
"../../dependencies/glm/"
"../../dependencies/imgui/"
"../../dependencies/imgui/cpp/"
"../../dependencies/imgui/impl/"
"../../dependencies/imgui-texteditor/"
"../../dependencies/json/include/"
"../../dependencies/lua/src/"
"../../dependencies/openal/include/"
"../../dependencies/portable-file-dialogs/include/"
"../../dependencies/sol3/single/include/"
"../../dependencies/stb/"
"../../dependencies/zlib/include/"
"../../dependencies/Box2D-MT/"
"../../dependencies/oggvorbis/include"
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
