project(deps C CXX)

set(dependencies_src
"../../dependencies/Box2D-MT/Box2D/Collision/b2BroadPhase.cpp"
"../../dependencies/Box2D-MT/Box2D/Collision/b2CollideCircle.cpp"
"../../dependencies/Box2D-MT/Box2D/Collision/b2CollideEdge.cpp"
"../../dependencies/Box2D-MT/Box2D/Collision/b2CollidePolygon.cpp"
"../../dependencies/Box2D-MT/Box2D/Collision/b2Collision.cpp"
"../../dependencies/Box2D-MT/Box2D/Collision/b2Distance.cpp"
"../../dependencies/Box2D-MT/Box2D/Collision/b2DynamicTree.cpp"
"../../dependencies/Box2D-MT/Box2D/Collision/b2TimeOfImpact.cpp"
"../../dependencies/Box2D-MT/Box2D/Collision/Shapes/b2ChainShape.cpp"
"../../dependencies/Box2D-MT/Box2D/Collision/Shapes/b2CircleShape.cpp"
"../../dependencies/Box2D-MT/Box2D/Collision/Shapes/b2EdgeShape.cpp"
"../../dependencies/Box2D-MT/Box2D/Collision/Shapes/b2PolygonShape.cpp"
"../../dependencies/Box2D-MT/Box2D/Common/b2BlockAllocator.cpp"
"../../dependencies/Box2D-MT/Box2D/Common/b2Draw.cpp"
"../../dependencies/Box2D-MT/Box2D/Common/b2Math.cpp"
"../../dependencies/Box2D-MT/Box2D/Common/b2Settings.cpp"
"../../dependencies/Box2D-MT/Box2D/Common/b2StackAllocator.cpp"
"../../dependencies/Box2D-MT/Box2D/Common/b2Timer.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/b2Body.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/b2ContactManager.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/b2Fixture.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/b2Island.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/b2World.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/b2WorldCallbacks.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Contacts/b2CircleContact.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Contacts/b2Contact.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Contacts/b2ContactSolver.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Contacts/b2PolygonContact.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2DistanceJoint.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2FrictionJoint.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2GearJoint.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2Joint.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2MotorJoint.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2MouseJoint.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2PulleyJoint.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2RopeJoint.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2WeldJoint.cpp"
"../../dependencies/Box2D-MT/Box2D/Dynamics/Joints/b2WheelJoint.cpp"
"../../dependencies/Box2D-MT/Box2D/MT/b2MtUtil.cpp"
"../../dependencies/Box2D-MT/Box2D/MT/b2Task.cpp"
"../../dependencies/Box2D-MT/Box2D/MT/b2ThreadPool.cpp"
"../../dependencies/Box2D-MT/Box2D/Rope/b2Rope.cpp"
"../../dependencies/glad/glad/glad.c"
"../../dependencies/googletest/googletest/src/gtest-death-test.cc"
"../../dependencies/googletest/googletest/src/gtest-filepath.cc"
"../../dependencies/googletest/googletest/src/gtest-matchers.cc"
"../../dependencies/googletest/googletest/src/gtest-port.cc"
"../../dependencies/googletest/googletest/src/gtest-printers.cc"
"../../dependencies/googletest/googletest/src/gtest-test-part.cc"
"../../dependencies/googletest/googletest/src/gtest-typed-test.cc"
"../../dependencies/googletest/googletest/src/gtest.cc"
"../../dependencies/imgui-texteditor/TextEditor.cpp"
"../../dependencies/imgui/cpp/imgui_stdlib.cpp"
"../../dependencies/imgui/imgui.cpp"
"../../dependencies/imgui/imgui_draw.cpp"
"../../dependencies/imgui/imgui_widgets.cpp"
"../../dependencies/imgui/impl/imgui_impl_glfw.cpp"
"../../dependencies/imgui/impl/imgui_impl_opengl3.cpp"
"../../dependencies/lua/src/lapi.c"
"../../dependencies/lua/src/lauxlib.c"
"../../dependencies/lua/src/lbaselib.c"
"../../dependencies/lua/src/lcode.c"
"../../dependencies/lua/src/lcorolib.c"
"../../dependencies/lua/src/lctype.c"
"../../dependencies/lua/src/ldblib.c"
"../../dependencies/lua/src/ldebug.c"
"../../dependencies/lua/src/ldo.c"
"../../dependencies/lua/src/ldump.c"
"../../dependencies/lua/src/lfunc.c"
"../../dependencies/lua/src/lgc.c"
"../../dependencies/lua/src/linit.c"
"../../dependencies/lua/src/liolib.c"
"../../dependencies/lua/src/llex.c"
"../../dependencies/lua/src/lmathlib.c"
"../../dependencies/lua/src/lmem.c"
"../../dependencies/lua/src/loadlib.c"
"../../dependencies/lua/src/lobject.c"
"../../dependencies/lua/src/lopcodes.c"
"../../dependencies/lua/src/loslib.c"
"../../dependencies/lua/src/lparser.c"
"../../dependencies/lua/src/lstate.c"
"../../dependencies/lua/src/lstring.c"
"../../dependencies/lua/src/lstrlib.c"
"../../dependencies/lua/src/ltable.c"
"../../dependencies/lua/src/ltablib.c"
"../../dependencies/lua/src/ltm.c"
"../../dependencies/lua/src/lundump.c"
"../../dependencies/lua/src/lutf8lib.c"
"../../dependencies/lua/src/lvm.c"
"../../dependencies/lua/src/lzio.c"
"../../dependencies/googletest/googletest/src/gtest-internal-inl.h"
)

source_group(${PROJECT_NAME} ${dependencies_src})
add_library(${PROJECT_NAME} STATIC ${dependencies_src})

target_include_directories(${PROJECT_NAME} PUBLIC
"../../dependencies/glad/"
"../../dependencies/glfw3/include/"
"../../dependencies/imgui/"
"../../dependencies/imgui/cpp/"
"../../dependencies/imgui/impl/"
"../../dependencies/imgui-texteditor/"
"../../dependencies/lua/src/"
"../../dependencies/googletest/googletest/include/"
"../../dependencies/googletest/googletest/"
"../../dependencies/Box2D-MT/"
"../pulsar/src/"
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

)

target_link_options(${PROJECT_NAME} PRIVATE
)

endif()
