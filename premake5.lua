-- premake5 build script.

---
--- START WORKSPACE
---
workspace "starlight"
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
		libdirs { "libs/libxml2/x32", "libs/allegro/x32/lib", "libs/allegro_deps/x32" }

	filter "platforms:Win64"
		architecture "x64"
		defines { "WIN64" }
		libdirs { "libs/libxml2/x64", "libs/allegro/x64/lib", "libs/allegro_deps/x64" }

	filter "platforms:Linux32"
		architecture "x32"
		libdirs { "/usr/lib/i386-linux-gnu/" }
		
	filter "platforms:Linux64"
		architecture "x64"
		libdirs { "/usr/lib/x86_64-linux-gnu/" }
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
		includedirs { "starlight/source", "libs/libxml2/include", "libs/allegro/include", "libs/allegro_deps/include" }
		links { "libxml2", "opengl32", "dumb", "FLAC", "freetype", "jpeg", "libpng16", "ogg", "opus", "opusfile", "physfs", "theoradec", "vorbis", "vorbisfile", "zlib", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid" }

	filter { "platforms:Win32 or Win64", "configurations:Debug"}
		links { "allegro-debug", "allegro_main-debug", "allegro_image-debug", "allegro_physfs-debug", "allegro_color-debug", "allegro_memfile-debug", "allegro_font-debug", "allegro_ttf-debug", "allegro_primitives-debug", "allegro_dialog-debug", "allegro_audio-debug", "allegro_acodec-debug", "allegro_video-debug" }

	filter { "platforms:Win32 or Win64", "configurations:Release"}
		links { "allegro_monolith-static", "winmm", "Shlwapi", "psapi", "dsound"}

	filter { "platforms:Linux32 or Linux64"}
		system "linux"
		includedirs { "starlight/source", "/usr/include/", "/usr/include/libxml2/" }
		links { "GL", "stdc++fs", "xml2", "allegro", "allegro_ttf", "allegro_font", "allegro_main", "allegro_audio", "allegro_color", "allegro_image", "allegro_video", "allegro_acodec", "allegro_dialog", "allegro_physfs", "allegro_memfile", "allegro_primitives", "physfs", "pthread", "dl", "z" }
		buildoptions { "-lstdc++fs" }
---
--- END SPECIFIC PLATFORM FILTERS
---


---
--- START PROJECTS
---
project "starlight"
	kind "StaticLib"
	location "starlight/"
	defines { "ALLEGRO_STATICLINK", "LIBXML_STATIC" }
	files { "starlight/source/**.hpp", "starlight/source/**.cpp", "starlight/source/**.h", "starlight/source/**.c" }

project "sandbox"
	kind "ConsoleApp"
	location "sandbox/"
	defines { "ALLEGRO_STATICLINK", "LIBXML_STATIC" }
	links { "starlight" }
	dependson { "starlight" }
	files { "sandbox/source/**.hpp", "sandbox/source/**.cpp" }
---
--- END PROJECTS
---