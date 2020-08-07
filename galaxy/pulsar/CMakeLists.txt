project(pulsar CXX)

set(
pulsar_src
"src/pulsar/detail/LogColours.cpp"
"src/pulsar/detail/Unix.cpp"
"src/pulsar/detail/Windows.cpp"
"src/pulsar/Log.cpp"
"src/pulsar/detail/LogColours.hpp"
"src/pulsar/detail/Unix.hpp"
"src/pulsar/detail/Windows.hpp"
"src/pulsar/Log.hpp"
)

source_group(TREE "" FILES ${pulsar_src})
add_library("pulsar" STATIC ${pulsar_src})
