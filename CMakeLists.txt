# Setup
cmake_minimum_required(VERSION 3.17)

# project
project(galaxy_engine LANGUAGES C CXX)

# Warning
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new build directory and run CMake from there.\n")
endif()

# Notifications
message("Starting galaxy_engine configuration. Building for: ${CMAKE_BUILD_TYPE}")

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

# GCC/MSVC compiler options.
if (MSVC)
	set(VS_MINIMUM_SDK_VERSION 10.0.19041.0)
    set(CMAKE_SYSTEM_VERSION 10.0.19041.0)
    set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
    set(FORCE_STATIC_VCRT ON CACHE BOOL "" FORCE)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(GALAXY_FREETYPE_HEADERS "${CMAKE_SOURCE_DIR}/dependencies/freetype2-win/include")
	set(GALAXY_FREETYPE_LIB "${CMAKE_SOURCE_DIR}/dependencies/freetype2-win/release static/win64/freetype.lib")
	set(ALSOFT_REQUIRE_DSOUND ON CACHE BOOL "" FORCE)
else()
	find_package(Threads REQUIRED)
	find_package(Freetype REQUIRED)
	add_definitions(-DCMAKE_EXPECTED_FREETYPE_VERSION="${FREETYPE_VERSION_STRING}")
	set(GALAXY_FREETYPE_HEADERS "${FREETYPE_INCLUDE_DIRS}")
	set(GALAXY_FREETYPE_LIB "${FREETYPE_LIBRARIES}")
	set(ALSOFT_STATIC_LIBGCC ON CACHE BOOL "" FORCE)
	set(ALSOFT_STATIC_STDCXX ON CACHE BOOL "" FORCE)
	set(ALSOFT_REQUIRE_ALSA ON CACHE BOOL "" FORCE)
	set(ALSOFT_REQUIRE_PULSEAUDIO OFF CACHE BOOL "" FORCE)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_source_files_properties(*.c LANGUAGE C)
set_source_files_properties(*.cc LANGUAGE CXX)
set_source_files_properties(*.cpp LANGUAGE CXX)
set_source_files_properties(*.h LANGUAGE C)
set_source_files_properties(*.hh LANGUAGE CXX)
set_source_files_properties(*.hpp LANGUAGE CXX)

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

add_dependencies(deps pulsar)
add_dependencies(protostar deps)
add_dependencies(frb protostar)
add_dependencies(quasar protostar)
add_dependencies(redshift protostar)
add_dependencies(solar protostar)
add_dependencies(starlight protostar)
add_dependencies(starmap protostar)
add_dependencies(galaxy
    frb
    quasar
    redshift
    solar
    starlight
    starmap
)

set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
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
set(OGG_LIBRARY "${CMAKE_SOURCE_DIR}/output/bin/ogg/${CMAKE_BUILD_TYPE}/ogg" CACHE STRING "" FORCE)

add_subdirectory(dependencies/vorbis)
add_dependencies(vorbis ogg)
set_target_properties(vorbis PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
)

set_target_properties(vorbisenc PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
)

set_target_properties(vorbisfile PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/vorbis/"
)

set(ALSOFT_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_NO_CONFIG_UTIL ON CACHE BOOL "" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_TESTS OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ALSOFT_INSTALL_UTILS OFF CACHE BOOL "" FORCE)
set(ALSOFT_UPDATE_BUILD_VERSION OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/openal)
set_target_properties(OpenAL PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/"
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/"
)

set_target_properties(common PROPERTIES
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

# Set dependencies to not show compiler errors.
if (MSVC)
	target_compile_options(glfw PUBLIC /W0 /experimental:external /external:anglebrackets /external:W0)
	target_compile_options(ogg PUBLIC /W0 /experimental:external /external:anglebrackets /external:W0)
	target_compile_options(vorbis PUBLIC /W0 /experimental:external /external:anglebrackets /external:W0)
	target_compile_options(vorbisenc PUBLIC /W0 /experimental:external /external:anglebrackets /external:W0)
	target_compile_options(vorbisfile PUBLIC /W0 /experimental:external /external:anglebrackets /external:W0)
	target_compile_options(OpenAL PUBLIC /EHsc /W0 /experimental:external /external:anglebrackets /external:W0)
	target_compile_options(common PUBLIC /EHsc /W0 /experimental:external /external:anglebrackets /external:W0)
	target_compile_options(zlibstatic PUBLIC /W0 /experimental:external /external:anglebrackets /external:W0)
endif()

# Configure post CMake-gen.
# This fixes in-source builds for these libs.
set(SIZE16 int16_t)
set(USIZE16 uint16_t)
set(SIZE32 int32_t)
set(USIZE32 uint32_t)
set(SIZE64 int64_t)
set(USIZE64 uint64_t)
configure_file(${CMAKE_SOURCE_DIR}/dependencies/ogg/include/ogg/config_types.h.in ${CMAKE_SOURCE_DIR}/dependencies/ogg/include/ogg/config_types.h @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/dependencies/zlib-conf/zconf.h ${CMAKE_SOURCE_DIR}/dependencies/zlib/zconf.h @ONLY)

# Meta trget to build external libs.
add_custom_target(BUILD_EXTERNAL_LIBS)
add_dependencies(BUILD_EXTERNAL_LIBS common glfw ogg vorbis vorbisenc vorbisfile OpenAL zlibstatic)

# Meta target to build internal libs.
add_custom_target(BUILD_INTERNAL_LIBS)
add_dependencies(BUILD_INTERNAL_LIBS BUILD_EXTERNAL_LIBS pulsar deps protostar frb quasar redshift solar starlight starmap)

# Meta target to build galaxy engine.
add_custom_target(BUILD_GALAXY_ENGINE)
add_dependencies(BUILD_GALAXY_ENGINE BUILD_INTERNAL_LIBS galaxy)

# Import applications
add_subdirectory(supercluster)
add_subdirectory(sandbox_tests)
add_subdirectory(tests)

add_dependencies(supercluster
    BUILD_EXTERNAL_LIBS
    BUILD_GALAXY_ENGINE
)

add_dependencies(sandbox_test
    BUILD_EXTERNAL_LIBS
    BUILD_GALAXY_ENGINE
)

add_dependencies(tests
    BUILD_EXTERNAL_LIBS
    BUILD_GALAXY_ENGINE
)

# Meta ALL target
add_custom_target(BUILD_ALL)
add_dependencies(supercluster sandbox_test tests BUILD_GALAXY_ENGINE)

# Set header directories.
set(SHARED_HEADERS
	"${GALAXY_FREETYPE_HEADERS}"
	"${CMAKE_SOURCE_DIR}/dependencies/Box2D-MT"
	"${CMAKE_SOURCE_DIR}/dependencies/c++20"
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
	"${CMAKE_SOURCE_DIR}/dependencies/rh-hash/src/include"
	"${CMAKE_SOURCE_DIR}/galaxy/frb/src"
	"${CMAKE_SOURCE_DIR}/galaxy/galaxy/src"
	"${CMAKE_SOURCE_DIR}/galaxy/protostar/src"
	"${CMAKE_SOURCE_DIR}/galaxy/pulsar/src"
	"${CMAKE_SOURCE_DIR}/galaxy/quasar/src"
	"${CMAKE_SOURCE_DIR}/galaxy/redshift/src"
	"${CMAKE_SOURCE_DIR}/galaxy/solar/src"
	"${CMAKE_SOURCE_DIR}/galaxy/starlight/src"
	"${CMAKE_SOURCE_DIR}/galaxy/starmap/src"
	"${CMAKE_SOURCE_DIR}/sandbox_tests/src"
	"${CMAKE_SOURCE_DIR}/supercluster/src"		
)

if (MSVC)
    set(ALL_HEADERS
    	"${SHARED_HEADERS}"
    )
else()
    set(ALL_HEADERS
    	"${SHARED_HEADERS}"
        "${CMAKE_SOURCE_DIR}/dependencies/c++20/atomic"
	)
endif()

target_include_directories(deps PUBLIC ${ALL_HEADERS})
target_include_directories(frb PUBLIC ${ALL_HEADERS})
target_include_directories(galaxy PUBLIC ${ALL_HEADERS})
target_include_directories(protostar PUBLIC ${ALL_HEADERS})
target_include_directories(pulsar PUBLIC ${ALL_HEADERS})
target_include_directories(quasar PUBLIC ${ALL_HEADERS})
target_include_directories(redshift PUBLIC ${ALL_HEADERS})
target_include_directories(solar PUBLIC ${ALL_HEADERS})
target_include_directories(starlight PUBLIC ${ALL_HEADERS})
target_include_directories(starmap PUBLIC ${ALL_HEADERS})
target_include_directories(supercluster PUBLIC ${ALL_HEADERS})
target_include_directories(sandbox_test PUBLIC ${ALL_HEADERS})
target_include_directories(tests PUBLIC ${ALL_HEADERS})

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
else()
    set(SYSTEM_LIBS
	    "pthread"
		"dl"
		"GL"
        "m"
        "c++"
        "c++abi"
        "c"
        "gcc_s"
        "gcc"
    )
endif()

# Setup compile and linking options.
include(cmake/CompileDefs.cmake)
include(cmake/LinkDefs.cmake)
include(cmake/PreprocessorDefs.cmake)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	target_compile_definitions(deps PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(frb PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(galaxy PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(protostar PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(pulsar PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(quasar PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(redshift PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(solar PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(starlight PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(starmap PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(supercluster PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(sandbox_test PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(tests PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})

	target_compile_options(deps PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(frb PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(galaxy PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(protostar PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(pulsar PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(quasar PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(redshift PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(solar PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(starlight PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(starmap PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(supercluster PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(sandbox_test PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(tests PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})

	target_link_options(deps PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(frb PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(galaxy PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(protostar PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(pulsar PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(quasar PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(redshift PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(solar PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(starlight PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(starmap PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(supercluster PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(sandbox_test PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(tests PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})

    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Debug/glfw3")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/ogg/Debug/ogg")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Debug/vorbis")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Debug/vorbisenc")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Debug/vorbisfile")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/common")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/OpenAL32")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Debug/zlibstaticd")

    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/deps/Debug/deps")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/frb/Debug/frb")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Debug/galaxy")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/protostar/Debug/protostar")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/pulsar/Debug/pulsar")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/quasar/Debug/quasar")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/redshift/Debug/redshift")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/solar/Debug/solar")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starlight/Debug/starlight")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starmap/Debug/starmap")

    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Debug/glfw3")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/ogg/Debug/ogg")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Debug/vorbis")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Debug/vorbisenc")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Debug/vorbisfile")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/common")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/OpenAL32")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Debug/zlibstaticd")

    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/deps/Debug/deps")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/frb/Debug/frb")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Debug/galaxy")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/protostar/Debug/protostar")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/pulsar/Debug/pulsar")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/quasar/Debug/quasar")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/redshift/Debug/redshift")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/solar/Debug/solar")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starlight/Debug/starlight")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starmap/Debug/starmap")

    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Debug/glfw3")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/ogg/Debug/ogg")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Debug/vorbis")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Debug/vorbisenc")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Debug/vorbisfile")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/common")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/OpenAL32")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Debug/zlibstaticd")

    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/deps/Debug/deps")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/frb/Debug/frb")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Debug/galaxy")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/protostar/Debug/protostar")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/pulsar/Debug/pulsar")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/quasar/Debug/quasar")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/redshift/Debug/redshift")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/solar/Debug/solar")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starlight/Debug/starlight")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starmap/Debug/starmap")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	target_compile_definitions(deps PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(frb PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(galaxy PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(protostar PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(pulsar PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(quasar PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(redshift PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(solar PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(starlight PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(starmap PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(supercluster PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(sandbox_test PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(tests PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})

	target_compile_options(deps PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(frb PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(galaxy PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(protostar PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(pulsar PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(quasar PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(redshift PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(solar PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(starlight PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(starmap PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(supercluster PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(sandbox_test PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(tests PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})

	target_link_options(deps PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(frb PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(galaxy PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(protostar PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(pulsar PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(quasar PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(redshift PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(solar PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(starlight PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(starmap PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(supercluster PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(sandbox_test PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(tests PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})

    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Release/glfw3")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/ogg/Release/ogg")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Release/vorbis")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Release/vorbisenc")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Release/vorbisfile")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/common")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/OpenAL32")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Release/zlibstatic")

    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/deps/Release/deps")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/frb/Release/frb")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Release/galaxy")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/protostar/Release/protostar")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/pulsar/Release/pulsar")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/quasar/Release/quasar")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/redshift/Release/redshift")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/solar/Release/solar")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starlight/Release/starlight")
    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starmap/Release/starmap")

    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Release/glfw3")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/ogg/Release/ogg")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Release/vorbis")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Release/vorbisenc")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Release/vorbisfile")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/common")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/OpenAL32")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Release/zlibstatic")

    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/deps/Release/deps")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/frb/Release/frb")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Release/galaxy")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/protostar/Release/protostar")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/pulsar/Release/pulsar")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/quasar/Release/quasar")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/redshift/Release/redshift")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/solar/Release/solar")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starlight/Release/starlight")
    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starmap/Release/starmap")

    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Release/glfw3")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/ogg/Release/ogg")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Release/vorbis")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Release/vorbisenc")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/vorbis/Release/vorbisfile")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/common")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/OpenAL32")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Release/zlibstatic")

    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/deps/Release/deps")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/frb/Release/frb")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Release/galaxy")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/protostar/Release/protostar")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/pulsar/Release/pulsar")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/quasar/Release/quasar")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/redshift/Release/redshift")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/solar/Release/solar")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starlight/Release/starlight")
    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/starmap/Release/starmap")
else()
	message(FATAL_ERROR "Could not determine build configuration. Is currently: ${CMAKE_BUILD_TYPE}")
endif()

if (NOT MSVC)
	target_link_libraries(supercluster PUBLIC Threads::Threads)
	target_link_libraries(sandbox_test PUBLIC Threads::Threads)
	target_link_libraries(tests PUBLIC Threads::Threads)
endif()

target_link_libraries(supercluster PUBLIC "${GALAXY_FREETYPE_LIB}")
target_link_libraries(sandbox_test PUBLIC "${GALAXY_FREETYPE_LIB}")
target_link_libraries(tests PUBLIC "${GALAXY_FREETYPE_LIB}")
target_link_libraries(supercluster PUBLIC ${SYSTEM_LIBS})
target_link_libraries(sandbox_test PUBLIC ${SYSTEM_LIBS})
target_link_libraries(tests PUBLIC ${SYSTEM_LIBS})

# Doxygen
include(cmake/Doxygen.cmake)