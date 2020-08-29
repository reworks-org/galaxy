# Setup
cmake_minimum_required(VERSION 3.17)

if (WIN32)
	set(VS_MINIMUM_SDK_VERSION 10.0.19041.0)
    set(CMAKE_SYSTEM_VERSION 10.0.19041.0)
endif()

# project
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
set_source_files_properties(*.h LANGUAGE C)
set_source_files_properties(*.hh LANGUAGE CXX)
set_source_files_properties(*.hpp LANGUAGE CXX)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "")
elseif (GNU)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)
endif()

# doxygen
include(cmake/Doxygen.cmake)

# Add internal projects.
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

# Add external dependencies.
add_subdirectory(dependencies/freetype2)
set_target_properties(freetype PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/freetype/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/freetype/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/freetype/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/freetype/"
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/glfw3)
set_target_properties(glfw PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/glfw/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/glfw/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/glfw/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/glfw/"
)

set(INSTALL_DOCS OFF CACHE BOOL "" FORCE)
set(INSTALL_PKG_CONFIG_MODULE OFF CACHE BOOL "" FORCE)
set(INSTALL_CMAKE_PACKAGE_MODULE OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/ogg)
set_target_properties(ogg PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/ogg/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/ogg/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/ogg/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/ogg/"
)

set(OGG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/ogg/include" CACHE STRING "" FORCE)
set(OGG_LIBRARY "${CMAKE_SOURCE_DIR}/output/bin/ogg/" CACHE STRING "" FORCE)
add_subdirectory(dependencies/vorbis)
add_dependencies(vorbis ogg)
set_target_properties(vorbis PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
)

set(ALSOFT_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_NO_CONFIG_UTIL ON CACHE BOOL "" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_TESTS OFF CACHE BOOL "" FORCE)
if (MSVC)
	set(FORCE_STATIC_VCRT ON CACHE BOOL "" FORCE)
	set(ALSOFT_REQUIRE_DSOUND ON CACHE BOOL "" FORCE)
else()
	set(ALSOFT_STATIC_LIBGCC ON CACHE BOOL "" FORCE)
	set(ALSOFT_STATIC_STDCXX ON CACHE BOOL "" FORCE)
	set(ALSOFT_REQUIRE_ALSA ON CACHE BOOL "" FORCE)
	set(ALSOFT_REQUIRE_PULSEAUDIO ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(dependencies/openal)
set_target_properties(OpenAL PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/"
)

add_subdirectory(dependencies/zlib)
set_target_properties(zlibstatic PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib/"
)

# Meta trget to build external libs.
add_custom_target(BUILD_EXTERNAL_LIBS)
add_dependencies(BUILD_EXTERNAL_LIBS freetype glfw ogg vorbis OpenAL zlibstatic)

# Meta target to build internal libs.
add_custom_target(BUILD_INTERNAL_LIBS)
add_dependencies(BUILD_INTERNAL_LIBS BUILD_EXTERNAL_LIBS pulsar deps protostar frb quasar redshift solar starlight starmap)

# Meta target to build galaxy engine.
add_custom_target(BUILD_GALAXY_ENGINE)
add_dependencies(BUILD_GALAXY_ENGINE BUILD_INTERNAL_LIBS galaxy)

# Import applications
add_subdirectory(supercluster)
add_subdirectory(tests)

add_dependencies(supercluster
    BUILD_EXTERNAL_LIBS
    BUILD_INTERNAL_LIBS
)

add_dependencies(tests
    BUILD_EXTERNAL_LIBS
    BUILD_INTERNAL_LIBS
)

# Set header directories.
set(all_header_dirs
	"${CMAKE_SOURCE_DIR}/dependencies/Box2D-MT"
	"${CMAKE_SOURCE_DIR}/dependencies/c++20"
	"${CMAKE_SOURCE_DIR}/dependencies/freetype2/include"
	"${CMAKE_SOURCE_DIR}/dependencies/glad"
	"${CMAKE_SOURCE_DIR}/dependencies/glfw3/include"
	"${CMAKE_SOURCE_DIR}/dependencies/glm"
	"${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest/include"
	"${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest/"
	"${CMAKE_SOURCE_DIR}/dependencies/imgui"
	"${CMAKE_SOURCE_DIR}/dependencies/imgui/cpp"
	"${CMAKE_SOURCE_DIR}/dependencies/imgui/impl"
	"${CMAKE_SOURCE_DIR}/dependencies/imgui-texteditor"
	"${CMAKE_SOURCE_DIR}/dependencies/json/include"
	"${CMAKE_SOURCE_DIR}/dependencies/lua/src"
	"${CMAKE_SOURCE_DIR}/dependencies/ogg/include"
	"${CMAKE_SOURCE_DIR}/dependencies/openal/include"
	"${CMAKE_SOURCE_DIR}/dependencies/pfd"
	"${CMAKE_SOURCE_DIR}/dependencies/sol3/single/include"
	"${CMAKE_SOURCE_DIR}/dependencies/stb"
	"${CMAKE_SOURCE_DIR}/dependencies/vorbis/include"
	"${CMAKE_SOURCE_DIR}/dependencies/zlib"
	"${CMAKE_SOURCE_DIR}/galaxy/frb/src"
	"${CMAKE_SOURCE_DIR}/galaxy/galaxy/src"
	"${CMAKE_SOURCE_DIR}/galaxy/protostar/src"
	"${CMAKE_SOURCE_DIR}/galaxy/pulsar/src"
	"${CMAKE_SOURCE_DIR}/galaxy/quasar/src"
	"${CMAKE_SOURCE_DIR}/galaxy/redshift/src"
	"${CMAKE_SOURCE_DIR}/galaxy/solar/src"
	"${CMAKE_SOURCE_DIR}/galaxy/starlight/src"
	"${CMAKE_SOURCE_DIR}/galaxy/starmap/src"
	"${CMAKE_SOURCE_DIR}/supercluster/src"
)

# generate ogg config header.
set(SIZE16 int16_t)
set(USIZE16 uint16_t)
set(SIZE32 int32_t)
set(USIZE32 uint32_t)
set(SIZE64 int64_t)
set(USIZE64 uint64_t)
configure_file(${CMAKE_SOURCE_DIR}/dependencies/ogg/include/ogg/config_types.h.in ${CMAKE_SOURCE_DIR}/dependencies/ogg/include/ogg/config_types.h @ONLY)

# fix zlib in source build.
configure_file(${CMAKE_SOURCE_DIR}/dependencies/zlib-conf/zconf.h ${CMAKE_SOURCE_DIR}/dependencies/zlib/zconf.h @ONLY)

target_include_directories(deps PUBLIC ${all_header_dirs})
target_include_directories(frb PUBLIC ${all_header_dirs})
target_include_directories(galaxy PUBLIC ${all_header_dirs})
target_include_directories(protostar PUBLIC ${all_header_dirs})
target_include_directories(pulsar PUBLIC ${all_header_dirs})
target_include_directories(quasar PUBLIC ${all_header_dirs})
target_include_directories(redshift PUBLIC ${all_header_dirs})
target_include_directories(solar PUBLIC ${all_header_dirs})
target_include_directories(starlight PUBLIC ${all_header_dirs})
target_include_directories(starmap PUBLIC ${all_header_dirs})
target_include_directories(supercluster PUBLIC ${all_header_dirs})
target_include_directories(tests PUBLIC ${all_header_dirs})

# System specific libraries.
if (WIN32)
    set(SYSTEM_LIBS
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
        "synchronization"
    )
elseif (GNU)
    set(SYSTEM_LIBS
        "pthread"
		"dl"
		"GL"
	)
endif()

target_link_libraries(supercluster PUBLIC freetype)
target_link_libraries(supercluster PUBLIC glfw)
target_link_libraries(supercluster PUBLIC ogg)
target_link_libraries(supercluster PUBLIC vorbis)
target_link_libraries(supercluster PUBLIC OpenAL)
target_link_libraries(supercluster PUBLIC zlibstatic)

target_link_libraries(supercluster PUBLIC deps)
target_link_libraries(supercluster PUBLIC frb)
target_link_libraries(supercluster PUBLIC galaxy)
target_link_libraries(supercluster PUBLIC protostar)
target_link_libraries(supercluster PUBLIC pulsar)
target_link_libraries(supercluster PUBLIC quasar)
target_link_libraries(supercluster PUBLIC redshift)
target_link_libraries(supercluster PUBLIC solar)
target_link_libraries(supercluster PUBLIC starlight)
target_link_libraries(supercluster PUBLIC starmap)

# Link library libs.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/freetype/Debug/freetyped.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Debug/glfw3.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/ogg/Debug/ogg.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Debug/vorbis.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/OpenAL32.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Debug/zlibstaticd.lib")

    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/deps/Debug/deps.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/frb/Debug/frb.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Debug/galaxy.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/protostar/Debug/protostar.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/pulsar/Debug/pulsar.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/quasar/Debug/quasar.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/redshift/Debug/redshift.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/solar/Debug/solar.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starlight/Debug/starlight.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starmap/Debug/starmap.lib")

    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/freetype/Debug/freetyped.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Debug/glfw3.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/ogg/Debug/ogg.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Debug/vorbis.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/OpenAL32.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Debug/zlibstaticd.lib")

    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/deps/Debug/deps.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/frb/Debug/frb.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Debug/galaxy.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/protostar/Debug/protostar.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/pulsar/Debug/pulsar.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/quasar/Debug/quasar.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/redshift/Debug/redshift.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/solar/Debug/solar.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starlight/Debug/starlight.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starmap/Debug/starmap.lib")
else()
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/freetype/Release/freetyped.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Release/glfw3.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/ogg/Release/ogg.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Release/vorbis.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/OpenAL32.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Release/zlibstaticd.lib")

    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/deps/Release/deps.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/frb/Release/frb.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Release/galaxy.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/protostar/Release/protostar.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/pulsar/Release/pulsar.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/quasar/Release/quasar.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/redshift/Release/redshift.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/solar/Release/solar.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starlight/Release/starlight.lib")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starmap/Release/starmap.lib")

    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/freetype/Release/freetyped.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Release/glfw3.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/ogg/Release/ogg.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Release/vorbis.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/OpenAL32.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Release/zlibstaticd.lib")

    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/deps/Release/deps.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/frb/Release/frb.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Release/galaxy.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/protostar/Release/protostar.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/pulsar/Release/pulsar.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/quasar/Release/quasar.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/redshift/Release/redshift.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/solar/Release/solar.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starlight/Release/starlight.lib")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starmap/Release/starmap.lib")
endif()

# Link system libs.
target_link_libraries(supercluster PUBLIC ${SYSTEM_LIBS})
target_link_libraries(tests PUBLIC ${SYSTEM_LIBS})
