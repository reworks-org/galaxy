-- premake5 build script.

---
--- START WORKSPACE
---
workspace "Quasar"
	language "C++"
	cppdialect "C++17"
	platforms { "Win32", "Win64", "Linux32", "Linux64" }
	configurations {"Debug", "Release"}
---
--- END WORKSPACE
---


---
--- START ARCHITECTURE FILTERS
---
	filter "platforms:Win32"
		architecture "x32"
		defines { "WIN32" }
		libdirs { "libs/SDL2/lib/x32" }

	filter "platforms:Win64"
		architecture "x64"
		defines { "WIN64" }
		libdirs { "libs/SDL2/lib/x64" }

	filter "platforms:Linux32"
		architecture "x32"

	filter "platforms:Linux64"
		architecture "x64"
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
--- START SPECIFIC PLATFORM FILTERS
---
	filter { "platforms:Win32 or Win64" }
		system "windows"
		systemversion "latest"
		flags { "NoPCH" }
		defines { "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_DEPRECATE", "_CRT_NONSTDC_NO_DEPRECATE" }
		buildoptions { "/bigobj" }
		includedirs { "Quasar/src", "libs/SDL2/include" }
		links { "OpenGL32" }

	filter { "platforms:Linux32 or Linux64"}
		system "linux"
		includedirs { "Quasar/src", os.findheader("libsdl2-dev") }
		libdirs { os.findlib("libsdl2-2.0.0") }
		links { "GL", "dl", "pthread" }
---
--- END SPECIFIC PLATFORM FILTERS
---


---
--- START PROJECTS
---
project "Quasar"
	kind "StaticLib"
	location "build/Quasar"
	files { "Quasar/src/**.hpp", "Quasar/src/**.cpp", "Quasar/src/**.h", "Quasar/src/**.c" }
	links { "SDL2", "SDL2main" }

project "Sandbox"
	kind "ConsoleApp"
	defines {  }
	location "build/Sandbox"
	files { "Sandbox/src/**.hpp", "Sandbox/src/**.cpp" }
	links { "SDL2", "SDL2main", "Quasar" }
	dependson { "Quasar" }
	
project "Tests"
	kind "ConsoleApp"
	defines {  }
	location "build/Tests"
	includedirs { "Tests/src/googletest/" }
	files { "Tests/src/**.hpp", "Tests/src/**.cpp", "Tests/src/**.h", "Tests/src/**.cc" }
	links { "SDL2", "SDL2main", "Quasar" }
	dependson { "Quasar" }
---
--- END PROJECTS
---