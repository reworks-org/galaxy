project(solar CXX)

set(
solar_src
"src/solar/Config.cpp"
"src/solar/Config.hpp"
"src/solar/entity/Manager.cpp"
"src/solar/entity/Manager.hpp"
"src/solar/sets/ComponentSet.cpp"
"src/solar/sets/ComponentSet.hpp"
"src/solar/sets/EntitySet.cpp"
"src/solar/sets/EntitySet.hpp"
"src/solar/system/System.cpp"
"src/solar/system/System.hpp"
)

source_group(TREE "" FILES ${solar_src})
add_library("solar" STATIC ${solar_src})
