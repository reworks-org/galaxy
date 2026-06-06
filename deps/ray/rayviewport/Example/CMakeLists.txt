cmake_minimum_required(VERSION 3.28)
include(FetchContent)



# Raylib
FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.zip
)
#FetchContent_GetProperties(raylib)
set(FETCHCONTENT_QUIET NO)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
FetchContent_MakeAvailable(raylib)

include_directories(${CMAKE_SOURCE_DIR}/..)


set(ProjectName Viewport_Example)
project(${ProjectName} LANGUAGES C)
#set(CMAKE_CXX_STANDARD 17)
file(GLOB Viewport_SRC_FILES "ViewportExample.c")
add_executable(${ProjectName} ${Viewport_SRC_FILES})
target_link_libraries(${ProjectName} raylib)
target_compile_definitions(${ProjectName} PUBLIC SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
