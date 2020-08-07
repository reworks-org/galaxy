project(starmap CXX)

set(starmap_src
"src/starmap/detail/Decoder.cpp"
"src/starmap/detail/Decoder.hpp"
"src/starmap/layer/GroupLayer.cpp"
"src/starmap/layer/GroupLayer.hpp"
"src/starmap/layer/ImageLayer.cpp"
"src/starmap/layer/ImageLayer.hpp"
"src/starmap/layer/Layer.cpp"
"src/starmap/layer/Layer.hpp"
"src/starmap/layer/ObjectLayer.cpp"
"src/starmap/layer/ObjectLayer.hpp"
"src/starmap/layer/TileLayer.cpp"
"src/starmap/layer/TileLayer.hpp"
"src/starmap/Map.cpp"
"src/starmap/Map.hpp"
"src/starmap/Meta.cpp"
"src/starmap/Meta.hpp"
"src/starmap/tile/Tile.cpp"
"src/starmap/tile/Tile.hpp"
"src/starmap/tile/TileOffset.cpp"
"src/starmap/tile/TileOffset.hpp"
"src/starmap/tile/Tileset.cpp"
"src/starmap/tile/Tileset.hpp"
"src/starmap/types/Chunk.cpp"
"src/starmap/types/Chunk.hpp"
"src/starmap/types/Frame.cpp"
"src/starmap/types/Frame.hpp"
"src/starmap/types/Grid.cpp"
"src/starmap/types/Grid.hpp"
"src/starmap/types/Object.cpp"
"src/starmap/types/Object.hpp"
"src/starmap/types/Point.cpp"
"src/starmap/types/Point.hpp"
"src/starmap/types/Property.cpp"
"src/starmap/types/Property.hpp"
"src/starmap/types/Terrain.cpp"
"src/starmap/types/Terrain.hpp"
"src/starmap/types/Text.cpp"
"src/starmap/types/Text.hpp"
"src/starmap/wangtile/WangColour.cpp"
"src/starmap/wangtile/WangColour.hpp"
"src/starmap/wangtile/WangSet.cpp"
"src/starmap/wangtile/WangSet.hpp"
"src/starmap/wangtile/WangTile.cpp"
"src/starmap/wangtile/WangTile.hpp"
)

source_group(${PROJECT_NAME} ${starmap_src})
add_library(${PROJECT_NAME} STATIC ${starmap_src})

target_include_directories(${PROJECT_NAME} PUBLIC
"src/"
"../../dependencies/json/include/"
"../../dependencies/zlib/include/"
"../pulsar/src/"
"../protostar/src"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
"$<$<CONFIG:Debug>:"
"_DEBUG"
">"
"$<$<CONFIG:Release>:"
"NDEBUG"
">"
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

)

target_link_options(${PROJECT_NAME} PRIVATE
)

endif()
