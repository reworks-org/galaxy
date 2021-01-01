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
    set(LIB_FILE_EXT "lib")
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
    set(LIB_FILE_EXT "a")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_source_files_properties(*.c LANGUAGE C)
set_source_files_properties(*.cc LANGUAGE CXX)
set_source_files_properties(*.cpp LANGUAGE CXX)
set_source_files_properties(*.h LANGUAGE C)
set_source_files_properties(*.hh LANGUAGE CXX)
set_source_files_properties(*.hpp LANGUAGE CXX)
set_source_files_properties(*.inl LANGUAGE CXX)

# Add internal projects.
add_subdirectory(dependencies)
add_subdirectory(galaxy)

# Doxygen
include(cmake/Doxygen.cmake)

if (NOT ENABLE_DOXYGEN)
	# Add external libs.
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

	set(ALSOFT_UTILS OFF CACHE BOOL "" FORCE)
	set(ALSOFT_NO_CONFIG_UTIL ON CACHE BOOL "" FORCE)
	set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(ALSOFT_TESTS OFF CACHE BOOL "" FORCE)
	set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF CACHE BOOL "" FORCE)
	set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(ALSOFT_INSTALL_UTILS OFF CACHE BOOL "" FORCE)
	set(ALSOFT_UPDATE_BUILD_VERSION OFF CACHE BOOL "" FORCE)
	set(CMAKE_DISABLE_FIND_PACKAGE_WindowsSDK ON CACHE BOOL "" FORCE)
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

	set(CMAKE_DISABLE_FIND_PACKAGE_WindowsSDK OFF CACHE BOOL "" FORCE)
	set(WITH_GZFILEOP ON CACHE BOOL "" FORCE)
	set(ZLIB_COMPAT ON CACHE BOOL "" FORCE)
	set(ZLIB_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
	set(ZLIB_DUAL_LINK OFF CACHE BOOL "" FORCE)
	set(WITH_SANITIZER OFF CACHE BOOL "" FORCE)
	set(WITH_FUZZERS OFF CACHE BOOL "" FORCE)
	set(WITH_OPTIM ON CACHE BOOL "" FORCE)
	set(WITH_NEW_STRATEGIES ON CACHE BOOL "" FORCE)
	set(WITH_NATIVE_INSTRUCTIONS OFF CACHE BOOL "" FORCE)
	set(WITH_MAINTAINER_WARNINGS OFF CACHE BOOL "" FORCE)
	set(WITH_CODE_COVERAGE OFF CACHE BOOL "" FORCE)
	set(WITH_INFLATE_STRICT OFF CACHE BOOL "" FORCE)
	set(WITH_INFLATE_ALLOW_INVALID_DIST OFF CACHE BOOL "" FORCE)
	set(WITH_UNALIGNED ON CACHE BOOL "" FORCE)
	add_subdirectory(dependencies/zlib-ng)
	set_target_properties(zlib PROPERTIES
	    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib/"
	    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib/"
	    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib/"
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib/"
	)

	set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
	add_subdirectory(dependencies/cppcoro)
	set_target_properties(cppcoro PROPERTIES
	    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/cppcoro/"
	    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/cppcoro/"
	    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/cppcoro/"
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/cppcoro/"
	)

	# Set dependencies to not show compiler errors.
	if (MSVC)
		target_compile_options(glfw PUBLIC /W0 /experimental:external /external:anglebrackets /external:W0)
		target_compile_options(OpenAL PUBLIC /EHsc /W0 /experimental:external /external:anglebrackets /external:W0)
		target_compile_options(common PUBLIC /EHsc /W0 /experimental:external /external:anglebrackets /external:W0)
		target_compile_options(zlib PUBLIC /W0 /experimental:external /external:anglebrackets /external:W0)
	    target_compile_options(cppcoro PUBLIC /W0 /experimental:external /external:anglebrackets /external:W0)
	endif()

	# Import applications
	add_subdirectory(sandbox_tests)
	add_subdirectory(tests)

	# Configure dependencies
	add_dependencies(galaxy glfw)
	add_dependencies(galaxy OpenAL)
	add_dependencies(galaxy common)
	add_dependencies(galaxy zlib)
	add_dependencies(galaxy cppcoro)
	add_dependencies(galaxy dependencies)
	add_dependencies(sandbox_test galaxy)
	add_dependencies(tests galaxy)

	# Set header directories.
	set(SHARED_HEADERS
	    "${CMAKE_SOURCE_DIR}/dependencies/cpp20/date/include"
	    "${CMAKE_SOURCE_DIR}/dependencies/cpp20/fmt/include"
		"${GALAXY_FREETYPE_HEADERS}"
		"${CMAKE_SOURCE_DIR}/dependencies/glad/glad"
		"${CMAKE_SOURCE_DIR}/dependencies/glfw3/include"
		"${CMAKE_SOURCE_DIR}/dependencies/glm"
		"${CMAKE_SOURCE_DIR}/dependencies/googletest/googlemock/include"
	    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googlemock"
	    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest/include"
		"${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest"
	    "${CMAKE_SOURCE_DIR}/dependencies/gsl/include/"
		"${CMAKE_SOURCE_DIR}/dependencies/imgui"
		"${CMAKE_SOURCE_DIR}/dependencies/json/include"
		"${CMAKE_SOURCE_DIR}/dependencies/lua/src"
		"${CMAKE_SOURCE_DIR}/dependencies/openal/include"
		"${CMAKE_SOURCE_DIR}/dependencies/pfd"
		"${CMAKE_SOURCE_DIR}/dependencies/rh-hash/src/include"
	    "${CMAKE_SOURCE_DIR}/dependencies/sol3/single/include"
		"${CMAKE_SOURCE_DIR}/dependencies/stb/include"
	    "${CMAKE_SOURCE_DIR}/dependencies/zconf"
	    "${CMAKE_SOURCE_DIR}/dependencies/zlib-ng"
	    "${CMAKE_SOURCE_DIR}/dependencies/cppcoro/include"
		"${CMAKE_SOURCE_DIR}/galaxy/src"
		"${CMAKE_SOURCE_DIR}/sandbox_tests/src"
	)
	set(ALL_HEADERS "${SHARED_HEADERS}")

	target_include_directories(dependencies PUBLIC ${ALL_HEADERS})
	target_include_directories(galaxy PUBLIC ${ALL_HEADERS})
	target_include_directories(sandbox_test PUBLIC ${ALL_HEADERS})
	target_include_directories(tests PUBLIC ${ALL_HEADERS})

	# System specific libraries.
	if (WIN32)
	    set(SYSTEM_LIBS
	        "opengl32.lib"
	        "kernel32.lib"
	        "user32.lib"
	        "gdi32.lib"
	        "winspool.lib"
	        "comdlg32.lib"
	        "advapi32.lib"
	        "shell32.lib"
	        "ole32.lib"
	        "oleaut32.lib"
	        "uuid.lib"
	        "winmm.lib"
	        "Shlwapi.lib"
	        "psapi.lib"
	        "dsound.lib"
	        "synchronization.lib"
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
		target_compile_definitions(dependencies PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
		target_compile_definitions(galaxy PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
		target_compile_definitions(sandbox_test PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
		target_compile_definitions(tests PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})

		target_compile_options(dependencies PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
		target_compile_options(galaxy PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
		target_compile_options(sandbox_test PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
		target_compile_options(tests PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})

		target_link_options(dependencies PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
		target_link_options(galaxy PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
		target_link_options(sandbox_test PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
		target_link_options(tests PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})

	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Debug/glfw3.${LIB_FILE_EXT}")
	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/common.${LIB_FILE_EXT}")
	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/OpenAL32.${LIB_FILE_EXT}")
	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Debug/zlibd.${LIB_FILE_EXT}")
	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/cppcoro/Debug/cppcoro.${LIB_FILE_EXT}")

	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Debug/dependencies.${LIB_FILE_EXT}")
	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Debug/galaxy.${LIB_FILE_EXT}")

	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Debug/glfw3.${LIB_FILE_EXT}")
	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/common.${LIB_FILE_EXT}")
	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/OpenAL32.${LIB_FILE_EXT}")
	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Debug/zlibd.${LIB_FILE_EXT}")
	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/cppcoro/Debug/cppcoro.${LIB_FILE_EXT}")

	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Debug/dependencies.${LIB_FILE_EXT}")
	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Debug/galaxy.${LIB_FILE_EXT}")
	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
		target_compile_definitions(dependencies PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
		target_compile_definitions(galaxy PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
		target_compile_definitions(sandbox_test PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
		target_compile_definitions(tests PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})

		target_compile_options(dependencies PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
		target_compile_options(galaxy PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
		target_compile_options(sandbox_test PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
		target_compile_options(tests PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})

		target_link_options(dependencies PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
		target_link_options(galaxy PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
		target_link_options(sandbox_test PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
		target_link_options(tests PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})

	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Release/glfw3.${LIB_FILE_EXT}")
	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/common.${LIB_FILE_EXT}")
	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/OpenAL32.${LIB_FILE_EXT}")
	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Release/zlib.${LIB_FILE_EXT}")
	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/cppcoro/Release/cppcoro.${LIB_FILE_EXT}")

	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Release/dependencies.${LIB_FILE_EXT}")
	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Release/galaxy.${LIB_FILE_EXT}")

	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Release/glfw3.${LIB_FILE_EXT}")
	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/common.${LIB_FILE_EXT}")
	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/OpenAL32.${LIB_FILE_EXT}")
	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Release/zlib.${LIB_FILE_EXT}")
	    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/cppcoro/Release/cppcoro.${LIB_FILE_EXT}")

	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Release/dependencies.${LIB_FILE_EXT}")
	    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Release/galaxy.${LIB_FILE_EXT}")
	else()
		message(FATAL_ERROR "Could not determine build configuration. Is currently: ${CMAKE_BUILD_TYPE}")
	endif()

	if (NOT MSVC)
		target_link_libraries(sandbox_test PUBLIC Threads::Threads)
		target_link_libraries(tests PUBLIC Threads::Threads)
	endif()

	target_link_libraries(sandbox_test PUBLIC "${GALAXY_FREETYPE_LIB}")
	target_link_libraries(tests PUBLIC "${GALAXY_FREETYPE_LIB}")
	target_link_libraries(sandbox_test PUBLIC ${SYSTEM_LIBS})
	target_link_libraries(tests PUBLIC ${SYSTEM_LIBS})
endif()
