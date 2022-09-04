cmake_minimum_required(VERSION 3.22)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new build directory and run CMake from there.\n")
endif()

project(galaxy LANGUAGES C CXX)
message("Starting galaxy configuration. Building for: ${CMAKE_BUILD_TYPE}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_source_files_properties(*.c LANGUAGE C)
set_source_files_properties(*.cc LANGUAGE CXX)
set_source_files_properties(*.cpp LANGUAGE CXX)
set_source_files_properties(*.h LANGUAGES C CXX)
set_source_files_properties(*.hh LANGUAGE CXX)
set_source_files_properties(*.hpp LANGUAGE CXX)
set_source_files_properties(*.inl LANGUAGE CXX)

set(CMAKE_CONFIGURATION_TYPES
	"Debug"
	"Release"
	CACHE STRING "" FORCE
)

find_package(Threads REQUIRED)
find_package(Freetype REQUIRED)
find_library(RT_LIBRARY rt)
find_library(MATH_LIBRARY m)
add_definitions(-DCMAKE_EXPECTED_FREETYPE_VERSION="${FREETYPE_VERSION_STRING}")

set(GALAXY_HEADERS
    "${FREETYPE_INCLUDE_DIRS}"
    "/usr/include"
    "${CMAKE_SOURCE_DIR}/lib/threadpool"
	"${CMAKE_SOURCE_DIR}/lib/box2d/include"
	"${CMAKE_SOURCE_DIR}/lib/entt/include"
	"${CMAKE_SOURCE_DIR}/lib/freetype2_win/include"
	"${CMAKE_SOURCE_DIR}/lib/glad/include"
	"${CMAKE_SOURCE_DIR}/lib/glfw/include"
	"${CMAKE_SOURCE_DIR}/lib/glm"
	"${CMAKE_SOURCE_DIR}/lib/googletest/googlemock/include"
	"${CMAKE_SOURCE_DIR}/lib/googletest/googletest"
	"${CMAKE_SOURCE_DIR}/lib/googletest/googletest/include"
	"${CMAKE_SOURCE_DIR}/lib/imgui"
	"${CMAKE_SOURCE_DIR}/lib/irrKlang/include"
	"${CMAKE_SOURCE_DIR}/lib/json/include"
	"${CMAKE_SOURCE_DIR}/lib/lua/src"
	"${CMAKE_SOURCE_DIR}/lib/magic_enum/include"
	"${CMAKE_SOURCE_DIR}/lib/msdfgl/include"
	"${CMAKE_SOURCE_DIR}/lib/pfd"
	"${CMAKE_SOURCE_DIR}/lib/rmlui/Include"
	"${CMAKE_SOURCE_DIR}/lib/sol3/include"
	"${CMAKE_SOURCE_DIR}/lib/stb/include"
	"${CMAKE_SOURCE_DIR}/lib/subprocess"
	"${CMAKE_SOURCE_DIR}/lib/robin_hood/src/include"
	"${CMAKE_SOURCE_DIR}/lib/zlib_ng/include"
	"${CMAKE_SOURCE_DIR}/lib/zlibcomplete/lib"
	"${CMAKE_SOURCE_DIR}/galaxy/src"
	"${CMAKE_SOURCE_DIR}/sandbox/src"
	"${CMAKE_SOURCE_DIR}/supercluster/src"
	"${CMAKE_SOURCE_DIR}/tests/src"
)

set(GALAXY_LIBS
    "${FREETYPE_LIBRARIES}"
	"${CMAKE_SOURCE_DIR}/lib/irrKlang/lib/libIrrKlang.so"
	Threads::Threads
	dl
	GL
	m
	rt
)

set(GALAXY_PREPROCESSOR_FLAGS_DEBUG
	MSDFGL_STATIC_DEFINE
	MSDFGEN_USE_CPP11
	RMLUI_STATIC_LIB
	IMGUI_IMPL_OPENGL_LOADER_GLAD
	GLM_FORCE_INLINE
	GLM_FORCE_INTRINSICS
	GLM_FORCE_XYZW_ONLY
	GLM_FORCE_SIZE_T_LENGTH
	GLM_FORCE_SILENT_WARNINGS
	GLM_FORCE_CTOR_INIT
	GLM_FORCE_RADIANS
	GLFW_INCLUDE_NONE
	STB_IMAGE_IMPLEMENTATION
	STB_IMAGE_WRITE_IMPLEMENTATION
	STB_IMAGE_RESIZE_IMPLEMENTATION
	STB_IMAGE_STATIC
	STB_IMAGE_WRITE_STATIC
	STB_IMAGE_RESIZE_STATIC
	SOL_CXX17_FEATURES
	_DEBUG
	_GLIBCXX_SANITIZE_VECTOR
	UNIX
	LINUX
	LINUX64
	UNIX64
	POISX
	POISX64
	_GLIBCXX_HAS_GTHREADS
	_GLIBCXX_USE_NANOSLEEP
	_GLIBCXX_PARALLEL
)

set(GALAXY_PREPROCESSOR_FLAGS_RELEASE
	MSDFGL_STATIC_DEFINE
	MSDFGEN_USE_CPP11
	RMLUI_STATIC_LIB
	IMGUI_IMPL_OPENGL_LOADER_GLAD
	GLM_FORCE_INLINE
	GLM_FORCE_INTRINSICS
	GLM_FORCE_XYZW_ONLY
	GLM_FORCE_SIZE_T_LENGTH
	GLM_FORCE_SILENT_WARNINGS
	GLM_FORCE_CTOR_INIT
	GLM_FORCE_RADIANS
	GLFW_INCLUDE_NONE
	STB_IMAGE_IMPLEMENTATION
	STB_IMAGE_WRITE_IMPLEMENTATION
	STB_IMAGE_RESIZE_IMPLEMENTATION
	STB_IMAGE_STATIC
	STB_IMAGE_WRITE_STATIC
	STB_IMAGE_RESIZE_STATIC
	SOL_CXX17_FEATURES
	NDEBUG
	_GLIBCXX_SANITIZE_VECTOR
	UNIX
	LINUX
	LINUX64
	UNIX64
	POISX
	POISX64
	_GLIBCXX_HAS_GTHREADS
	_GLIBCXX_USE_NANOSLEEP
	_GLIBCXX_PARALLEL
)

set(GALAXY_LINK_FLAGS_DEBUG
	-pthread
	-static
	-static-libgcc
	-static-libstdc++
	-pie
	-latomic
	-lstdc++fs
	-ltbb
)

set(GALAXY_LINK_FLAGS_RELEASE
	-pthread
	-static
	-static-libgcc
	-static-libstdc++
	-pie
	-latomic
	-lstdc++fs
	-ltbb
)

set(GALAXY_COMPILE_FLAGS_DEBUG
    -std=c++20
    -fchar8_t
    -fcoroutines
    -fno-modules-ts
    -fnew-inheriting-ctors
    -fnew-ttp-matching
    -frtti
    -fvisibility-ms-compat
    -pthread
    -fexceptions
    -pipe
    -fpie
    -pedantic
    -Wno-error
    -Wpedantic
    -Wctor-dtor-privacy
    -Wno-init-list-lifetime
    -Winvalid-imported-macros
    -Wnoexcept
    -Wclass-memaccess
    -Wold-style-cast
    -Woverloaded-virtual
    -Wsign-promo
    -Wno-mismatched-new-delete
    -Wzero-as-null-pointer-constant
    -Wextra-semi
    -Wsuggest-final-types
    -Wsuggest-final-methods
    -Wsuggest-override
    -Wuseless-cast
    -Wdouble-promotion
    -Weffc++
    -Wall
    -Wextra
    -fanalyzer
    -Og
    -fsanitize=address
)

set(GALAXY_COMPILE_FLAGS_RELEASE
    -std=c++20
    -fchar8_t
    -fcoroutines
    -Wnonnull
    -fno-modules-ts
    -fnew-inheriting-ctors
    -fnew-ttp-matching
    -frtti
    -fvisibility-ms-compat
    -pthread
    -fexceptions
    -pipe
    -fpie
    -pedantic
    -Wno-error
    -w
    -fno-pretty-templates
    -O3
)

set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
set_target_properties(glfw PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/glfw"
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/glfw"
	PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/glfw"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/glfw"
)

add_subdirectory(lib)
add_subdirectory(galaxy)
add_subdirectory(sandbox)
add_subdirectory(supercluster)
add_subdirectory(tests)

add_dependencies(libs glfw)
add_dependencies(galaxy libs)
add_dependencies(sandbox galaxy)
add_dependencies(supercluster galaxy)
add_dependencies(tests galaxy)

target_include_directories(libs PUBLIC ${GALAXY_HEADERS})
target_include_directories(galaxy PUBLIC ${GALAXY_HEADERS})
target_include_directories(sandbox PUBLIC ${GALAXY_HEADERS})
target_include_directories(supercluster PUBLIC ${GALAXY_HEADERS})
target_include_directories(tests PUBLIC ${GALAXY_HEADERS})

target_link_libraries(libs PUBLIC ${GALAXY_LIBS})
target_link_libraries(galaxy PUBLIC ${GALAXY_LIBS})
target_link_libraries(sandbox PUBLIC ${GALAXY_LIBS})
target_link_libraries(supercluster PUBLIC ${GALAXY_LIBS})
target_link_libraries(tests PUBLIC ${GALAXY_LIBS})

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	target_compile_definitions(libs PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(galaxy PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(sandbox PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(supercluster PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
	target_compile_definitions(tests PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})

	target_compile_options(libs PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(galaxy PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(sandbox PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(supercluster PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
	target_compile_options(tests PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})

	target_link_options(libs PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(galaxy PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(sandbox PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(supercluster PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
	target_link_options(tests PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})

	target_link_libraries(libs PUBLIC "${CMAKE_SOURCE_DIR}/output/glfw/Debug/libglfw3.a")
	target_link_libraries(galaxy PUBLIC "${CMAKE_SOURCE_DIR}/output/glfw/Debug/libglfw3.a")
	target_link_libraries(sandbox PUBLIC "${CMAKE_SOURCE_DIR}/output/glfw/Debug/libglfw3.a")
	target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/glfw/Debug/libglfw3.a")
	target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/glfw/Debug/libglfw3.a")

	target_link_libraries(galaxy PUBLIC "${CMAKE_SOURCE_DIR}/output/libs/Debug/liblibs.a")
    target_link_libraries(sandbox PUBLIC "${CMAKE_SOURCE_DIR}/output/libs/Debug/liblibs.a")
	target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/libs/Debug/liblibs.a")
	target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/libs/Debug/liblibs.a")

    target_link_libraries(sandbox PUBLIC "${CMAKE_SOURCE_DIR}/output/galaxy/Debug/libgalaxy.a")
	target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/galaxy/Debug/libgalaxy.a")
	target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/galaxy/Debug/libgalaxy.a")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	target_compile_definitions(libs PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(galaxy PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(sandbox PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(supercluster PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
	target_compile_definitions(tests PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})

	target_compile_options(libs PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(galaxy PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(sandbox PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(supercluster PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
	target_compile_options(tests PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})

	target_link_options(libs PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(galaxy PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(sandbox PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(supercluster PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
	target_link_options(tests PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})

	target_link_libraries(libs PUBLIC "${CMAKE_SOURCE_DIR}/output/glfw/Release/libglfw3.a")
	target_link_libraries(galaxy PUBLIC "${CMAKE_SOURCE_DIR}/output/glfw/Release/libglfw3.a")
	target_link_libraries(sandbox PUBLIC "${CMAKE_SOURCE_DIR}/output/glfw/Release/libglfw3.a")
	target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/glfw/Release/libglfw3.a")
	target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/glfw/Release/libglfw3.a")

	target_link_libraries(galaxy PUBLIC "${CMAKE_SOURCE_DIR}/output/libs/Release/liblibs.a")
	target_link_libraries(sandbox PUBLIC "${CMAKE_SOURCE_DIR}/output/libs/Release/liblibs.a")
	target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/libs/Release/liblibs.a")
	target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/libs/Release/liblibs.a")

	target_link_libraries(sandbox PUBLIC "${CMAKE_SOURCE_DIR}/output/galaxy/Release/libgalaxy.a")
	target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/galaxy/Release/libgalaxy.a")
	target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/galaxy/Release/libgalaxy.a")
else()
	message(FATAL_ERROR "Could not determine build configuration. Is currently: ${CMAKE_BUILD_TYPE}")
endif()

target_compile_options(glfw PUBLIC -w)
target_compile_options(libs PUBLIC -w)
target_compile_options(sandbox PUBLIC -w)
target_compile_options(tests PUBLIC -w)