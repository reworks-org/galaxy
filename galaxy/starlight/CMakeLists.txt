project(starlight CXX)

set(
starlight_src
"src/starlight/detail/Meta.cpp"
"src/starlight/detail/Storage.cpp"
"src/starlight/Dispatcher.cpp"
"src/starlight/detail/Meta.hpp"
"src/starlight/detail/Storage.hpp"
"src/starlight/Dispatcher.hpp"
)

source_group(TREE "" FILES ${starlight_src})
add_library("starlight" STATIC ${starlight_src})
