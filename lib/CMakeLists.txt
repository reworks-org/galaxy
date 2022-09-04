project(libs C CXX)

file(GLOB_RECURSE libs_src
    "${CMAKE_SOURCE_DIR}/lib/box2d/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/lib/glad/glad.c"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googlemock/src/gmock-cardinalities.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googlemock/src/gmock-internal-utils.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googlemock/src/gmock-matchers.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googlemock/src/gmock-spec-builders.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googlemock/src/gmock.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googletest/src/gtest-assertion-result.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googletest/src/gtest-death-test.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googletest/src/gtest-filepath.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googletest/src/gtest-matchers.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googletest/src/gtest-port.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googletest/src/gtest-printers.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googletest/src/gtest-test-part.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googletest/src/gtest-typed-test.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googletest/src/gtest.cc"
    "${CMAKE_SOURCE_DIR}/lib/googletest/googletest/src/gtest-internal-inl.h"
    "${CMAKE_SOURCE_DIR}/lib/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/lib/lua/src/*.c"
    "${CMAKE_SOURCE_DIR}/lib/msdfgl/src/*.c"
    "${CMAKE_SOURCE_DIR}/lib/msdfgl/src/*.c"
    "${CMAKE_SOURCE_DIR}/lib/rmlui/Source/Core/*.cpp"
    "${CMAKE_SOURCE_DIR}/lib/rmlui/Source/Debugger/*.cpp"
    "${CMAKE_SOURCE_DIR}/lib/rmlui/Source/Lua/*.cpp"
    "${CMAKE_SOURCE_DIR}/lib/stb/stb_vorbis.c"
    "${CMAKE_SOURCE_DIR}/lib/stb/stb_vorbis.c"
    "${CMAKE_SOURCE_DIR}/lib/zlibcomplete/lib/*.cpp"
)

source_group(${PROJECT_NAME} ${libs_src})
add_library(${PROJECT_NAME} STATIC ${libs_src})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/${PROJECT_NAME}"
)