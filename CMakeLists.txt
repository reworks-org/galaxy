cmake_minimum_required(VERSION 3.18)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new build directory and run CMake from there.\n")
endif()

# Force set cmake vars
if (MSVC)
	set(CMAKE_SYSTEM_VERSION 10.0.19041.0 CACHE STRING INTERNAL FORCE)
	set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION 10.0.19041.0 CACHE STRING INTERNAL FORCE)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON CACHE BOOL INTERNAL FORCE)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING INTERNAL FORCE)
	set(CMAKE_SHARED_LINKER_FLAGS_CHECKED ON CACHE BOOL INTERNAL FORCE)
	set(CMAKE_EXE_LINKER_FLAGS_CHECKED ON CACHE BOOL INTERNAL FORCE)
endif()

project(galaxy_engine LANGUAGES C CXX)
message("Starting galaxy_engine configuration. Building for: ${CMAKE_BUILD_TYPE}")

# File properties.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_source_files_properties(*.c LANGUAGE C)
set_source_files_properties(*.cc LANGUAGE CXX)
set_source_files_properties(*.cpp LANGUAGE CXX)
set_source_files_properties(*.h LANGUAGES C CXX)
set_source_files_properties(*.hh LANGUAGE CXX)
set_source_files_properties(*.hpp LANGUAGE CXX)
set_source_files_properties(*.inl LANGUAGE CXX)

# Options
option(GALAXY_ENABLE_DOXYGEN "Enable a target for building doxygen." OFF)
option(GALAXY_BUILD_TESTS "Enable a target for building unit and sandbox tests." ON)

# Doxygen.
if (${GALAXY_ENABLE_DOXYGEN})
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
        set(DOXYGEN_OUT ${CMAKE_SOURCE_DIR}/output/docs)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT}/Doxyfile @ONLY)
        add_custom_target(doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}/Doxyfile
            WORKING_DIRECTORY ${DOXYGEN_OUT}
            VERBATIM
        )
    endif()
else()
	# Set to default.
	set(CMAKE_EXE_LINKER_FLAGS_PROFILE "")
	set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "")
	set(CMAKE_C_FLAGS_DEBUG "")
	set(CMAKE_C_FLAGS_RELEASE "")
	set(CMAKE_CXX_FLAGS "")
	set(CMAKE_CXX_FLAGS_DEBUG "")
	set(CMAKE_CXX_FLAGS_RELEASE "")
	set(CMAKE_EXE_LINKER_FLAGS "")
	set(CMAKE_MODULE_LINKER_FLAGS "")
	set(CMAKE_SHARED_LINKER_FLAGS "")
	set(CMAKE_STATIC_LINKER_FLAGS "")
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
	set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
	set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
	set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
	set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
	set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")

	# Language config.
	set(CMAKE_C_STANDARD 11)
	set(CMAKE_C_STANDARD_REQUIRED True)
	set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED True)

	# Configure properties.
	set(CMAKE_CONFIGURATION_TYPES
		"Debug"
		"Release"
		CACHE STRING "" FORCE
	)

	if (MSVC)
		set(LIB_FILE_EXT "lib")
		set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
		set(FORCE_STATIC_VCRT ON CACHE BOOL "" FORCE)
		
		set(GALAXY_EXTERNAL_HEADERS
			"${CMAKE_SOURCE_DIR}/dependencies/freetype2-win/include"
			"${CMAKE_SOURCE_DIR}/dependencies/nvidia/nvapi/include"
		)
		
		set(GALAXY_PRECOMPILED_LIBS
			"${CMAKE_SOURCE_DIR}/dependencies/freetype2-win/release static/win64/freetype.${LIB_FILE_EXT}"
			"${CMAKE_SOURCE_DIR}/dependencies/nvidia/nvapi/lib/nvapi64.${LIB_FILE_EXT}"
			"${CMAKE_SOURCE_DIR}/dependencies/nvidia/hbaoplus/lib/GFSDK_SSAO_GL.win64.${LIB_FILE_EXT}"
		)
	else()
		set(LIB_FILE_EXT "a")
		
		find_package(Threads REQUIRED)
		find_package(Freetype REQUIRED)
		add_definitions(-DCMAKE_EXPECTED_FREETYPE_VERSION="${FREETYPE_VERSION_STRING}")

		set(GALAXY_EXTERNAL_HEADERS
			"${FREETYPE_INCLUDE_DIRS}"
			"/usr/include"
		)
		
		set(GALAXY_PRECOMPILED_LIBS
			"${FREETYPE_LIBRARIES}"
			"${CMAKE_SOURCE_DIR}/dependencies/nvidia/hbaoplus/lib/GFSDK_SSAO_GL.x86_64-linux-gnu.${LIB_FILE_EXT}"
		)
	endif()

	# Add internal projects.
	add_subdirectory(dependencies)

	# Add external libs.
	set(SFW_WITH_TEST OFF CACHE BOOL "" FORCE)
	add_subdirectory(dependencies/simplefilewatcher)
	set_target_properties(sfw_lib PROPERTIES
	    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher"
	    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher"
	    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher"
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher"
	)

	set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	add_subdirectory(dependencies/glfw3)
	set_target_properties(glfw PROPERTIES
	    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/glfw"
	    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/glfw"
	    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/glfw"
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/glfw"
	)

	set(ALSOFT_UTILS OFF CACHE BOOL "" FORCE)
	set(ALSOFT_NO_CONFIG_UTIL ON CACHE BOOL "" FORCE)
	set(ALSOFT_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(ALSOFT_INSTALL_UTILS OFF CACHE BOOL "" FORCE)
	set(ALSOFT_UPDATE_BUILD_VERSION OFF CACHE BOOL "" FORCE)
	add_subdirectory(dependencies/openal)
	set_target_properties(OpenAL PROPERTIES
	    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL"
	    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL"
	    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL"
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL"
	)

	set_target_properties(common PROPERTIES
	    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL"
	    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL"
	    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL"
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/OpenAL"
	)

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
	    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib"
	    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib"
	    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib"
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/zlib"
	)

	set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
	set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
	set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
	set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
	set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
	set(ASSIMP_IGNORE_GIT_HASH ON CACHE BOOL "" FORCE)
	add_subdirectory(dependencies/assimp)
	set_target_properties(assimp PROPERTIES
	    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/assimp"
	    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/assimp"
	    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/assimp"
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/assimp"
	    PREFIX ""
	    OUTPUT_NAME "assimp"
	    SUFFIX ".${LIB_FILE_EXT}"
	)
	set_target_properties(IrrXML PROPERTIES
	    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/IrrXML"
	    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/IrrXML"
	    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/IrrXML"
	    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/IrrXML"
	    PREFIX ""
	    OUTPUT_NAME "IrrXML"
	    SUFFIX ".${LIB_FILE_EXT}"
	)

	#https://github.com/NVIDIAGameWorks/PhysX/issues/249
	#set(TARGET_BUILD_PLATFORM "windows")
	#if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	#	set(NV_USE_DEBUG_WINCRT ON CACHE BOOL "Use the debug version of the CRT")
	#else()
	#	set(NV_USE_DEBUG_WINCRT OFF CACHE BOOL "Use the debug version of the CRT")
	#endif()
	#set(PHYSX_ROOT_DIR "${CMAKE_SOURCE_DIR}/dependencies/nvidia/physx/physx")
	#set(PHYSX_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dependencies/nvidia/physx/physx/include ${CMAKE_SOURCE_DIR}/dependencies/nvidia/physx/pxshared/include/)
	#set(PHYSX_LIBRARIES
	#	PhysX;
	#	PhysXCharacterKinematic;
	#	PhysXCommon;
	#	PhysXCooking;
	#	PhysXExtensions;
	#	PhysXVehicle;
	#	LowLevel;
	#	LowLevelAABB;
	#	LowLevelDynamics;
	#	SceneQuery;
	#	SimulationController;
	#	FastXml;
	#	PhysXPvdSDK;
	#	PhysXTask;
	#	PhysXFoundation
	#)
	#set(PHYSX_LIBRARIES_OUTPUTS
	#	PhysX;
	#	PhysXCharacterKinematic;
	#	PhysXCommon;
	#	PhysXCooking;
	#	PhysXExtensions;
	#	PhysXFoundation;
	#	PhysXPvdSDK;
	#	PhysXVehicle
	#)
	#set(PXSHARED_PATH "${CMAKE_SOURCE_DIR}/dependencies/nvidia/physx/pxshared")
	#set(PXSHARED_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
	#set(CMAKEMODULES_VERSION "1.27")
	#set(CMAKEMODULES_PATH "${CMAKE_SOURCE_DIR}/dependencies/nvidia/physx/externals/cmakemodules")
	#set(PX_OUTPUT_LIB_DIR "${CMAKE_SOURCE_DIR}/output/bin/physx")
	#set(PX_OUTPUT_BIN_DIR "${CMAKE_SOURCE_DIR}/output/bin/physx")
	#set(PX_BUILDSNIPPETS OFF CACHE BOOL "" FORCE)
	#set(PX_BUILDPUBLICSAMPLES OFF CACHE BOOL "" FORCE)
	#set(PX_GENERATE_STATIC_LIBRARIES ON CACHE BOOL "" FORCE)
	#set(PX_FLOAT_POINT_PRECISE_MATH OFF CACHE BOOL "" FORCE)
	#set(NV_USE_STATIC_WINCRT ON CACHE BOOL "" FORCE)
	#set(PX_CMAKE_SUPPRESS_REGENERATION OFF CACHE BOOL "" FORCE)
	#set(PX_COPY_EXTERNAL_DLL OFF CACHE BOOL "" FORCE)
	#file(COPY ${CMAKE_SOURCE_DIR}/cmake/patches/PxPreprocessor.h DESTINATION ${CMAKE_SOURCE_DIR}/dependencies/nvidia/physx/pxshared/include/foundation)
	#add_subdirectory(dependencies/nvidia/physx/physx/compiler/public)
	#foreach (target IN LISTS PHYSX_LIBRARIES)
	#	target_compile_options(${target} PUBLIC /MT /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
	#	set_target_properties(${target} PROPERTIES
	#		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/physx"
	#    	LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/output/bin/physx/Debug"
	#    	LIBRARY_OUTPUT_DIRECTORY_CHECKED "${CMAKE_SOURCE_DIR}/output/bin/physx/Checked"
	#    	LIBRARY_OUTPUT_DIRECTORY_PROFILE "${CMAKE_SOURCE_DIR}/output/bin/physx/Profile"
	#    	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/output/bin/physx/Release"
	#    	
	#    	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/physx"
	#    	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/output/bin/physx/Debug"
	#    	ARCHIVE_OUTPUT_DIRECTORY_CHECKED "${CMAKE_SOURCE_DIR}/output/bin/physx/Checked"
	#    	ARCHIVE_OUTPUT_DIRECTORY_PROFILE "${CMAKE_SOURCE_DIR}/output/bin/physx/Profile"
	#    	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/output/bin/physx/Release"
	#    	
	#    	PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/physx"
	#    	PDB_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/output/bin/physx/Debug"
	#    	PDB_OUTPUT_DIRECTORY_CHECKED "${CMAKE_SOURCE_DIR}/output/bin/physx/Checked"
	#    	PDB_OUTPUT_DIRECTORY_PROFILE "${CMAKE_SOURCE_DIR}/output/bin/physx/Profile"
	#    	PDB_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/output/bin/physx/Release"
	#    	
	#    	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin/physx"
	#    	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/output/bin/physx/Debug"
	#    	RUNTIME_OUTPUT_DIRECTORY_CHECKED "${CMAKE_SOURCE_DIR}/output/bin/physx/Checked"
	#    	RUNTIME_OUTPUT_DIRECTORY_PROFILE "${CMAKE_SOURCE_DIR}/output/bin/physx/Profile"
	#    	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/output/bin/physx/Release"
	#	)
	#endforeach()

	# Set external project options.
	if (MSVC)
		if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
			target_compile_options(sfw_lib PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} /MTd /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(glfw PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} /MTd /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(OpenAL PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} /MTd /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(common PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} /MTd /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(zlib PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} /MTd /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(IrrXML PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} /MTd /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(assimp PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} /MTd /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(assimp_zlibstatic PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} /MTd /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)

			target_link_options(sfw_lib PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
			target_link_options(glfw PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
			target_link_options(OpenAL PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
			target_link_options(common PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
			target_link_options(zlib PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
			target_link_options(IrrXML PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
			target_link_options(assimp PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
			target_link_options(assimp_zlibstatic PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
		elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
			target_compile_options(sfw_lib PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} /MT /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(glfw PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} /MT /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(OpenAL PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} /MT /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(common PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} /MT /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(zlib PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} /MT /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(IrrXML PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} /MT /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(assimp PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} /MT /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(assimp_zlibstatic PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} /MT /WX- /EHsc /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)

			target_link_options(sfw_lib PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
			target_link_options(glfw PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
			target_link_options(OpenAL PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
			target_link_options(common PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
			target_link_options(zlib PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
			target_link_options(IrrXML PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
			target_link_options(assimp PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
			target_link_options(assimp_zlibstatic PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
		else()
			message(FATAL_ERROR "Could not determine build configuration. Is currently: ${CMAKE_BUILD_TYPE}")
		endif()
	else()
		if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
				target_compile_options(sfw_lib PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} -w)
				target_compile_options(glfw PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} -w)
				target_compile_options(OpenAL PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} -w)
				target_compile_options(common PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} -w)
				target_compile_options(zlib PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} -w)
				target_compile_options(IrrXML PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} -w)
				target_compile_options(assimp PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} -w)
				target_compile_options(assimp_zlibstatic PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG} -w)

				target_link_options(sfw_lib PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
				target_link_options(glfw PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
				target_link_options(OpenAL PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
				target_link_options(common PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
				target_link_options(zlib PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
				target_link_options(IrrXML PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
				target_link_options(assimp PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
				target_link_options(assimp_zlibstatic PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
			elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
				target_compile_options(sfw_lib PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} -w)
				target_compile_options(glfw PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} -w)
				target_compile_options(OpenAL PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} -w)
				target_compile_options(common PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} -w)
				target_compile_options(zlib PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} -w)
				target_compile_options(IrrXML PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} -w)
				target_compile_options(assimp PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} -w)
				target_compile_options(assimp_zlibstatic PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE} -w)

				target_link_options(sfw_lib PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
				target_link_options(glfw PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
				target_link_options(OpenAL PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
				target_link_options(common PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
				target_link_options(zlib PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
				target_link_options(IrrXML PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
				target_link_options(assimp PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
				target_link_options(assimp_zlibstatic PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
			else()
				message(FATAL_ERROR "Could not determine build configuration. Is currently: ${CMAKE_BUILD_TYPE}")
			endif()
	endif()

	# Import projects.
	add_subdirectory(galaxy)
	add_subdirectory(supercluster)

	# Configure dependencies
	add_dependencies(galaxy sfw_lib)
	add_dependencies(galaxy glfw)
	add_dependencies(galaxy OpenAL)
	add_dependencies(galaxy common)
	add_dependencies(galaxy zlib)
	add_dependencies(galaxy IrrXML)
	add_dependencies(galaxy assimp)
	add_dependencies(galaxy assimp_zlibstatic)
	#foreach (target IN LISTS PHYSX_LIBRARIES_OUTPUTS)
	#	add_dependencies(galaxy ${target})
	#endforeach()

	add_dependencies(galaxy dependencies)
	add_dependencies(supercluster galaxy)

	# Set header directories.
	set(HEADERS
		"${GALAXY_EXTERNAL_HEADERS}"
		#"${PHYSX_INCLUDE_DIRS}"
		"${CMAKE_SOURCE_DIR}/dependencies/assimp/include"
		"${CMAKE_SOURCE_DIR}/dependencies/assimp-conf"
		"${CMAKE_SOURCE_DIR}/dependencies/base64"
		"${CMAKE_SOURCE_DIR}/dependencies/glad/glad"
		"${CMAKE_SOURCE_DIR}/dependencies/glfw3/include"
		"${CMAKE_SOURCE_DIR}/dependencies/glm"
		"${CMAKE_SOURCE_DIR}/dependencies/googletest/googlemock/include"
	    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googlemock"
	    "${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest/include"
		"${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest"
		"${CMAKE_SOURCE_DIR}/dependencies/imgui"
		"${CMAKE_SOURCE_DIR}/dependencies/json/include"
		"${CMAKE_SOURCE_DIR}/dependencies/lua/src"
		"${CMAKE_SOURCE_DIR}/dependencies/magic_enum/include"
		"${CMAKE_SOURCE_DIR}/dependencies/nvidia/hbaoplus/include"
		"${CMAKE_SOURCE_DIR}/dependencies/openal/include"
		"${CMAKE_SOURCE_DIR}/dependencies/pfd"
		"${CMAKE_SOURCE_DIR}/dependencies/rh-hash/src/include"
		"${CMAKE_SOURCE_DIR}/dependencies/simplefilewatcher/include"
	    "${CMAKE_SOURCE_DIR}/dependencies/sol3/single/include"
		"${CMAKE_SOURCE_DIR}/dependencies/stb/include"
	    "${CMAKE_SOURCE_DIR}/dependencies/zconf"
	    "${CMAKE_SOURCE_DIR}/dependencies/zlc/lib"
	    "${CMAKE_SOURCE_DIR}/dependencies/zlib-ng"
		"${CMAKE_SOURCE_DIR}/galaxy/src"
		"${CMAKE_SOURCE_DIR}/sandbox_tests/src"
		"${CMAKE_SOURCE_DIR}/sandbox_3d_tests/src"
		"${CMAKE_SOURCE_DIR}/supercluster/src"
		"${CMAKE_SOURCE_DIR}/tests/src"
	)

	target_include_directories(dependencies PUBLIC ${HEADERS})
	target_include_directories(galaxy PUBLIC ${HEADERS})
	target_include_directories(supercluster PUBLIC ${HEADERS})

	# Setup compile and linking options.
	include(cmake/CompileDefs.cmake)
	include(cmake/LinkDefs.cmake)
	include(cmake/PreprocessorDefs.cmake)

	# System specific libraries.
	if (MSVC)
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
		    Threads::Threads
		)
	endif()

	target_link_libraries(supercluster PUBLIC "${SYSTEM_LIBS}")
	target_link_libraries(supercluster PUBLIC "${GALAXY_PRECOMPILED_LIBS}")

	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		target_compile_definitions(dependencies PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
		target_compile_definitions(galaxy PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
		target_compile_definitions(supercluster PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})

		target_compile_options(dependencies PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
		target_compile_options(galaxy PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
		target_compile_options(supercluster PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})

		target_link_options(dependencies PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
		target_link_options(galaxy PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
		target_link_options(supercluster PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})

		target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher/Debug/sfw_lib.${LIB_FILE_EXT}")
		target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Debug/glfw3.${LIB_FILE_EXT}")
	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/common.${LIB_FILE_EXT}")
	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/OpenAL32.${LIB_FILE_EXT}")
	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Debug/zlibd.${LIB_FILE_EXT}")
	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/IrrXML/Debug/IrrXML.${LIB_FILE_EXT}")
	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/assimp/Debug/assimp.${LIB_FILE_EXT}")

		#foreach (target IN LISTS PHYSX_LIBRARIES_OUTPUTS)
		#	target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/physx/Debug/${target}_static.${LIB_FILE_EXT}")
		#endforeach()

	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Debug/dependencies.${LIB_FILE_EXT}")
	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Debug/galaxy.${LIB_FILE_EXT}")
	elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
		target_compile_definitions(dependencies PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
		target_compile_definitions(galaxy PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
		target_compile_definitions(supercluster PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})

		target_compile_options(dependencies PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
		target_compile_options(galaxy PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
		target_compile_options(supercluster PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})

		target_link_options(dependencies PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
		target_link_options(galaxy PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
		target_link_options(supercluster PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})

		target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher/Release/sfw_lib.${LIB_FILE_EXT}")
		target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Release/glfw3.${LIB_FILE_EXT}")
	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/common.${LIB_FILE_EXT}")
	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/OpenAL32.${LIB_FILE_EXT}")
	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Release/zlib.${LIB_FILE_EXT}")
	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/IrrXML/Release/IrrXML.${LIB_FILE_EXT}")
		target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/assimp/Release/assimp.${LIB_FILE_EXT}")

		#foreach (target IN LISTS PHYSX_LIBRARIES_OUTPUTS)
		#	target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/physx/Release/${target}_static.${LIB_FILE_EXT}")
		#endforeach()

	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Release/dependencies.${LIB_FILE_EXT}")
	    target_link_libraries(supercluster PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Release/galaxy.${LIB_FILE_EXT}")
	else()
		message(FATAL_ERROR "Could not determine build configuration. Is currently: ${CMAKE_BUILD_TYPE}")
	endif()

	if (MSVC)
		target_compile_options(dependencies PUBLIC /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
		target_compile_options(galaxy PUBLIC /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
		target_compile_options(supercluster PUBLIC /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
	else()
		target_compile_options(dependencies PUBLIC -w)
	endif()

	if (${GALAXY_BUILD_TESTS})
		add_subdirectory(sandbox_3d_tests)
		add_subdirectory(sandbox_tests)
		add_subdirectory(tests)

		add_dependencies(sandbox_3d_test galaxy)
		add_dependencies(sandbox_test galaxy)
		add_dependencies(tests galaxy)

		target_include_directories(sandbox_3d_test PUBLIC ${HEADERS})
		target_include_directories(sandbox_test PUBLIC ${HEADERS})
		target_include_directories(tests PUBLIC ${HEADERS})

		target_link_libraries(sandbox_3d_test PUBLIC "${SYSTEM_LIBS}")
		target_link_libraries(sandbox_test PUBLIC "${SYSTEM_LIBS}")
		target_link_libraries(tests PUBLIC "${SYSTEM_LIBS}")
		target_link_libraries(sandbox_3d_test PUBLIC "${GALAXY_PRECOMPILED_LIBS}")
		target_link_libraries(sandbox_test PUBLIC "${GALAXY_PRECOMPILED_LIBS}")
		target_link_libraries(tests PUBLIC "${GALAXY_PRECOMPILED_LIBS}")

		if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
			target_compile_definitions(sandbox_3d_test PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
			target_compile_definitions(sandbox_test PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})
			target_compile_definitions(tests PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_DEBUG})

			target_compile_options(sandbox_3d_test PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
			target_compile_options(sandbox_test PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})
			target_compile_options(tests PUBLIC ${GALAXY_COMPILE_FLAGS_DEBUG})

			target_link_options(sandbox_3d_test PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
			target_link_options(sandbox_test PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})
			target_link_options(tests PUBLIC ${GALAXY_LINK_FLAGS_DEBUG})

			target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher/Debug/sfw_lib.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Debug/glfw3.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/common.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/OpenAL32.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Debug/zlibd.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/IrrXML/Debug/IrrXML.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/assimp/Debug/assimp.${LIB_FILE_EXT}")

		    foreach (target IN LISTS PHYSX_LIBRARIES_OUTPUTS)
				target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/physx/Debug/${target}_static.${LIB_FILE_EXT}")
			endforeach()

		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Debug/dependencies.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Debug/galaxy.${LIB_FILE_EXT}")

			target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher/Debug/sfw_lib.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Debug/glfw3.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/common.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/OpenAL32.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Debug/zlibd.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/IrrXML/Debug/IrrXML.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/assimp/Debug/assimp.${LIB_FILE_EXT}")

		    #foreach (target IN LISTS PHYSX_LIBRARIES_OUTPUTS)
			#	target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/physx/Debug/${target}_static.${LIB_FILE_EXT}")
			#endforeach()

		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Debug/dependencies.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Debug/galaxy.${LIB_FILE_EXT}")
			
			target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher/Debug/sfw_lib.${LIB_FILE_EXT}")
			target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Debug/glfw3.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/common.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Debug/OpenAL32.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Debug/zlibd.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/IrrXML/Debug/IrrXML.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/assimp/Debug/assimp.${LIB_FILE_EXT}")

		    #foreach (target IN LISTS PHYSX_LIBRARIES_OUTPUTS)
			#	target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/physx/Debug/${target}_static.${LIB_FILE_EXT}")
			#endforeach()

		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Debug/dependencies.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Debug/galaxy.${LIB_FILE_EXT}")
		elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
			target_compile_definitions(sandbox_3d_test PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
			target_compile_definitions(sandbox_test PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})
			target_compile_definitions(tests PUBLIC ${GALAXY_PREPROCESSOR_FLAGS_RELEASE})

			target_compile_options(sandbox_3d_test PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
			target_compile_options(sandbox_test PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})
			target_compile_options(tests PUBLIC ${GALAXY_COMPILE_FLAGS_RELEASE})

			target_link_options(sandbox_3d_test PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
			target_link_options(sandbox_test PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})
			target_link_options(tests PUBLIC ${GALAXY_LINK_FLAGS_RELEASE})

			target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher/Release/sfw_lib.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Release/glfw3.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/common.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/OpenAL32.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Release/zlib.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/IrrXML/Release/IrrXML.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/assimp/Release/assimp.${LIB_FILE_EXT}")

		    #foreach (target IN LISTS PHYSX_LIBRARIES_OUTPUTS)
			#	target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/physx/Release/${target}_static.${LIB_FILE_EXT}")
			#endforeach()

		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Release/dependencies.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_3d_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Release/galaxy.${LIB_FILE_EXT}")

			target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher/Release/sfw_lib.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Release/glfw3.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/common.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/OpenAL32.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Release/zlib.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/IrrXML/Release/IrrXML.${LIB_FILE_EXT}")
			target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/assimp/Release/assimp.${LIB_FILE_EXT}")

			#foreach (target IN LISTS PHYSX_LIBRARIES_OUTPUTS)
			#	target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/physx/Release/${target}_static.${LIB_FILE_EXT}")
			#endforeach()

		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Release/dependencies.${LIB_FILE_EXT}")
		    target_link_libraries(sandbox_test PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Release/galaxy.${LIB_FILE_EXT}")

		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/simplefilewatcher/Release/sfw_lib.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/glfw/Release/glfw3.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/common.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/OpenAL/Release/OpenAL32.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/zlib/Release/zlib.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/IrrXML/Release/IrrXML.${LIB_FILE_EXT}")
			target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/assimp/Release/assimp.${LIB_FILE_EXT}")

			#foreach (target IN LISTS PHYSX_LIBRARIES_OUTPUTS)
			#	target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/physx/Release/${target}_static.${LIB_FILE_EXT}")
			#endforeach()

		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/dependencies/Release/dependencies.${LIB_FILE_EXT}")
		    target_link_libraries(tests PUBLIC "${CMAKE_SOURCE_DIR}/output/bin/galaxy/Release/galaxy.${LIB_FILE_EXT}")
		else()
			message(FATAL_ERROR "Could not determine build configuration. Is currently: ${CMAKE_BUILD_TYPE}")
		endif()

		if (MSVC)
			target_compile_options(sandbox_3d_test PUBLIC /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(sandbox_test PUBLIC /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
			target_compile_options(tests PUBLIC /W0 /experimental:external /external:anglebrackets /external:I /external:templates- /external:W0)
		else()
			target_compile_options(sandbox_3d_test PUBLIC -w)
			target_compile_options(sandbox_test PUBLIC -w)
			target_compile_options(tests PUBLIC -w)
		endif()
	endif()
endif()