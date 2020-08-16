# Setup
cmake_minimum_required(VERSION 3.17)
project(galaxy_engine LANGUAGES C CXX)

# Warning
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please make a new build directory and run CMake from there.\n")
endif()

# Reset.
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_MODULE_LINKER_FLAGS "")
set(CMAKE_SHARED_LINKER_FLAGS "")
set(CMAKE_STATIC_LINKER_FLAGS "")

# Default.
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CONFIGURATION_TYPES
"Debug"
"Release"
CACHE STRING "" FORCE
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_source_files_properties(*.c LANGUAGE C)
set_source_files_properties(*.cc LANGUAGE CXX)
set_source_files_properties(*.cpp LANGUAGE CXX)
#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;--header-filter=.* -p=${CMAKE_FILES_DIRECTORY}/ --config=")

if(NOT MSVC)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "")
endif()

set(DEPS_LIBS
"vorbisfile"
"vorbisenc"
"vorbis"
"ogg"
"libz-static"
"OpenAL32"
"glfw3"
"freetype"
)

if(WIN32)
    set(WIN_LIBS
        "opengl32"
        "kernel32"
        "user32"
        "gdi32"
        "winspool"
        "comdlg32"
        "advapi32"
        "shell32"
        "ole32"
        "oleaut32"
        "uuid"
        "winmm"
        "Shlwapi"
        "psapi"
        "dsound"
    )
endif()

# doxygen
include(cmake/Doxygen.cmake)

add_subdirectory(galaxy/deps)
add_subdirectory(galaxy/frb)
add_subdirectory(galaxy/galaxy)
add_subdirectory(galaxy/protostar)
add_subdirectory(galaxy/pulsar)
add_subdirectory(galaxy/quasar)
add_subdirectory(galaxy/redshift)
add_subdirectory(galaxy/solar)
add_subdirectory(galaxy/starlight)
add_subdirectory(galaxy/starmap)
add_subdirectory(supercluster)
add_subdirectory(tests)

add_dependencies(deps
pulsar
)

add_dependencies(protostar
deps
)

add_dependencies(frb
protostar
)

add_dependencies(quasar
protostar
)

add_dependencies(redshift
protostar
)

add_dependencies(solar
protostar
)

add_dependencies(starlight
protostar
)

add_dependencies(starmap
protostar
)

add_dependencies(galaxy
frb
quasar
redshift
solar
starlight
starmap
)

add_dependencies(supercluster
galaxy
)

add_dependencies(tests
galaxy
)

target_link_libraries(supercluster PUBLIC deps)
target_link_libraries(supercluster PUBLIC frb)
target_link_libraries(supercluster PUBLIC galaxy)
target_link_libraries(supercluster PUBLIC protostar)
target_link_libraries(supercluster PUBLIC quasar)
target_link_libraries(supercluster PUBLIC redshift)
target_link_libraries(supercluster PUBLIC solar)
target_link_libraries(supercluster PUBLIC starlight)
target_link_libraries(supercluster PUBLIC starmap)
target_link_libraries(supercluster PUBLIC "${DEPS_LIBS}")

target_link_libraries(tests PUBLIC deps)
target_link_libraries(tests PUBLIC frb)
target_link_libraries(tests PUBLIC galaxy)
target_link_libraries(tests PUBLIC protostar)
target_link_libraries(tests PUBLIC quasar)
target_link_libraries(tests PUBLIC redshift)
target_link_libraries(tests PUBLIC solar)
target_link_libraries(tests PUBLIC starlight)
target_link_libraries(tests PUBLIC starmap)
target_link_libraries(tests PUBLIC "${DEPS_LIBS}")

if(WIN32)
    target_link_libraries(supercluster PUBLIC "${WIN_LIBS}")
    target_link_libraries(tests PUBLIC "${WIN_LIBS}")
endif()

# target to only build libs
add_custom_target(BUILD_LIBS_ONLY)
add_dependencies(BUILD_LIBS_ONLY pulsar deps protostar frb quasar redshift solar starlight starmap galaxy)
