project(tests CXX)

set(
tests_src
"src/main.cpp"
"src/protostar/async/ProtectedArithmeticTest.cpp"
"src/protostar/async/TaskTest.cpp"
"src/protostar/async/ThreadPoolTest.cpp"
"src/protostar/graphics/ColourTest.cpp"
"src/protostar/graphics/RectTest.cpp"
"src/protostar/math/RandomTest.cpp"
"src/protostar/math/RectPackTest.cpp"
"src/protostar/res/ResourceCacheTest.cpp"
"src/protostar/state/StateMachineTest.cpp"
"src/protostar/state/StateTest.cpp"
"src/protostar/system/TimeTest.cpp"
"src/protostar/system/UniqueIDTest.cpp"
"src/solar/ConfigTest.cpp"
"src/solar/entity/ManagerTest.cpp"
"src/solar/sets/ComponentSetTest.cpp"
"src/solar/sets/EntitySetTest.cpp"
"src/starlight/DispatcherTest.cpp"
"src/starmap/MapTest.cpp"
)

source_group(TREE "" FILES ${tests_src})
add_library("tests" STATIC ${tests_src})
