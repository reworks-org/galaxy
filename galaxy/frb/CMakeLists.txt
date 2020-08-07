project(frb CXX)

set(
frb_src
"src/frb/audio/Music.cpp"
"src/frb/audio/Sound.cpp"
"src/frb/buffer/Buffer.cpp"
"src/frb/buffer/BufferStream.cpp"
"src/frb/Context.cpp"
"src/frb/detail/Error.cpp"
"src/frb/detail/Source.cpp"
"src/frb/detail/SourceManipulator.cpp"
"src/frb/audio/Music.hpp"
"src/frb/audio/Sound.hpp"
"src/frb/buffer/Buffer.hpp"
"src/frb/buffer/BufferStream.hpp"
"src/frb/Context.hpp"
"src/frb/detail/Error.hpp"
"src/frb/detail/Source.hpp"
"src/frb/detail/SourceManipulator.hpp"
)

source_group(TREE "" FILES ${frb_src})
add_library("frb" STATIC ${frb_src})
