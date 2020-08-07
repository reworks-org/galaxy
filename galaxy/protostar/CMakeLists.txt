project(protostar CXX)

set(protostar_src
"src/protostar/async/ProtectedArithmetic.cpp"
"src/protostar/async/ProtectedArithmetic.hpp"
"src/protostar/async/Task.cpp"
"src/protostar/async/Task.hpp"
"src/protostar/async/ThreadPool.cpp"
"src/protostar/async/ThreadPool.hpp"
"src/protostar/events/KeyCharEvent.cpp"
"src/protostar/events/KeyCharEvent.hpp"
"src/protostar/events/KeyDownEvent.cpp"
"src/protostar/events/KeyDownEvent.hpp"
"src/protostar/events/KeyUpEvent.cpp"
"src/protostar/events/KeyUpEvent.hpp"
"src/protostar/events/MouseMovedEvent.cpp"
"src/protostar/events/MouseMovedEvent.hpp"
"src/protostar/events/MousePressedEvent.cpp"
"src/protostar/events/MousePressedEvent.hpp"
"src/protostar/events/MouseReleasedEvent.cpp"
"src/protostar/events/MouseReleasedEvent.hpp"
"src/protostar/events/MouseWheelEvent.cpp"
"src/protostar/events/MouseWheelEvent.hpp"
"src/protostar/graphics/Colour.cpp"
"src/protostar/graphics/Colour.hpp"
"src/protostar/graphics/Rect.cpp"
"src/protostar/graphics/Rect.hpp"
"src/protostar/math/Maths.cpp"
"src/protostar/math/Maths.hpp"
"src/protostar/math/Random.cpp"
"src/protostar/math/Random.hpp"
"src/protostar/math/RectPack.cpp"
"src/protostar/math/RectPack.hpp"
"src/protostar/res/ResourceCache.cpp"
"src/protostar/res/ResourceCache.hpp"
"src/protostar/state/State.cpp"
"src/protostar/state/State.hpp"
"src/protostar/state/StateMachine.cpp"
"src/protostar/state/StateMachine.hpp"
"src/protostar/system/Concepts.cpp"
"src/protostar/system/Concepts.hpp"
"src/protostar/system/Keys.cpp"
"src/protostar/system/Keys.hpp"
"src/protostar/system/Time.cpp"
"src/protostar/system/Time.hpp"
"src/protostar/system/UniqueID.cpp"
"src/protostar/system/UniqueID.hpp"
)

source_group(${PROJECT_NAME} ${protostar_src})
add_library(${PROJECT_NAME} STATIC ${protostar_src})

target_include_directories(${PROJECT_NAME} PUBLIC
"src/"
"../pulsar/src/"
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
