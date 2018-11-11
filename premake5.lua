---
--- START WORKSPACE
---
workspace "Quasar"
	language "C++"
	platforms { "Win32", "Win64", "Linux32", "Linux64" }
	configurations {"Debug", "Release"}
---
--- END WORKSPACE
---


---
--- START PLATFORM FILTERS
---
	filter "platforms:Win32"
		system "windows"
		architecture "x32"
		defines { "WIN32" }
		includedirs { "Quasar/src", "libs/SDL2/include" }
		libdirs { "libs/SDL2/lib/x32" }

	filter "platforms:Win64"
		system "windows"
		architecture "x64"
		defines { "WIN64" }
		includedirs { "Quasar/src", "libs/SDL2/include" }
		libdirs { "libs/SDL2/lib/x64" }

	filter "platforms:Linux32"
		system "linux"
		architecture "x32"
		includedirs { "Quasar/src", os.findheader("libsdl2-dev") }
		libdirs { os.findlib("libsdl2-2.0.0") }

	filter "platforms:Linux64"
		system "linux"
		architecture "x64"
		includedirs { "Quasar/src", os.findheader("libsdl2-dev") }
		libdirs { os.findlib("libsdl2-2.0.0") }
---
--- END PLATFORM FILTERS
---


---
--- START CONFIGURATIONS
---
	filter "configurations:Debug"
		defines { "_DEBUG" }
		symbols "On"
		optimize "Debug"

	filter "configurations:Release"
		defines { "NDEBUG" }
		symbols "Off"
		optimize "Speed"	
---
--- END CONFIGURATIONS
---


---
--- START SPECIFIC FILTERS
---
	filter { "platforms:Win32 or Win64" }
		flags { "NoPCH" }
		defines { "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_DEPRECATE", "_CRT_NONSTDC_NO_DEPRECATE" }
		buildoptions { "/bigobj" }

	filter { "platforms:Win32 or Win64"}
		links { "OpenGL32" }

	filter { "platforms:Linux32 or Linux64"}
		links { "GL" }
---
--- END SPECIFIC FILTERS
---


---
--- START PROJECTS
---
project "Quasar"
	kind "StaticLib"
	language "C++"
	defines { "_LIB" }
	location "build/Quasar"
	files { "Quasar/src/**.hpp", "Quasar/src/**.cpp", "Quasar/src/**.h", "Quasar/src/**.c" }
	links { "SDL2", "SDL2main" }

project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	defines {  }
	location "build/Sandbox"
	files { "Sandbox/src/*.hpp", "Sandbox/src/*.cpp" }
	links { "SDL2", "SDL2main", "Quasar" }
	dependson { "Quasar" }
	
project "Tests"
	kind "ConsoleApp"
	language "C++"
	defines {  }
	location "build/Tests"
	includedirs { "Tests/src/googletest" }
	files { "Tests/src/*.h", "Tests/src/*.cc", "Tests/src/*.cpp" }
	links { "SDL2", "SDL2main", "Quasar" }
	dependson { "Quasar" }
---
--- END PROJECTS
---