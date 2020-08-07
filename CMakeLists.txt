# Setup
cmake_minimum_required(VERSION 3.15)
project
("galaxy"
VERSION 1.0.0
LANGUAGES C CXX
)

# Set vars
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CONFIGURATION_TYPES
"Debug"
"Release"
)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
