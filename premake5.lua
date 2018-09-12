-- premake5.lua

-- workaround
-- os.target()
-- --os=macosx 

workspace "starlight"
	platforms { "Win32", "Win64", "MacOS", "Linux" }
	configurations {"Debug", "Release"}
	location "build"

	filter "platforms:Win32"
		system "windows"
		architecture "x32"
		defines { "WIN32" }
		includedirs { "starlight/source", "libs/libxml2/include", "libs/allegro_deps/include", "libs/allegro/include" }
		libdirs { "libs/libxml2/x32", "libs/allegro_deps/x32", "libs/allegro/x32/lib" }

	filter "platforms:Win64"
		system "windows"
		architecture "x64"
		defines { "WIN64" }
		includedirs { "starlight/source", "libs/libxml2/include", "libs/allegro_deps/include", "libs/allegro/include" }
		libdirs { "libs/libxml2/x64", "libs/allegro_deps/x64", "libs/allegro/x64/lib" }

	filter "platforms:MacOS"
		system "macosx"
		architecture "x64"
		includedirs { "starlight/source" }
		libdirs { os.findlib("libxml2"), os.findlib("allegro") }
		links { "libxml2", "allegro" }
		postbuildcommands { "cd starlight && cp -r source headers/ && cd headers/ && rm -rf *.cpp && rm -rf *.c" }

	filter "platforms:Linux"
		system "linux"
		architecture "x64"
		includedirs { "starlight/source" }
		libdirs { os.findlib("libxml2"), os.findlib("liballegro5.2") }
		links { "libxml2", "liballegro5.2" }
		postbuildcommands { "cd starlight && cp -r source headers/ && cd headers/ && rm -rf *.cpp && rm -rf *.c" }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		symbols "On"
		optimize "Debug"

	filter "configurations:Release"
		defines { "NDEBUG" }
		symbols "Off"
		optimize "Speed"	

	filter { "platforms:Win32 or Win64" }
		flags { "NoPCH" }
		defines { "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_DEPRECATE", "_CRT_NONSTDC_NO_DEPRECATE" }
		buildoptions { "/bigobj" }
		postbuildcommands { "call cd $(SolutionDir)starlight && xcopy /e /v /i /y source headers && cd headers && del /s *.cpp && del /s *.c" }

	filter { "platforms:Win32 or Win64", "configurations:Debug"}
		links { "libxml2", "opengl32", "jpeg", "libpng16", "zlib", "freetype", "dumb", "FLAC", "vorbis", "vorbisfile", "ogg", "opus", "opusfile", "physfs", "theoradec", "allegro", "allegro_main-debug", "allegro_image-debug", "allegro_physfs-debug", "allegro_color-debug", "allegro_memfile-debug", "allegro_font-debug", "allegro_ttf-debug", "allegro_primitives-debug", "allegro_dialog-debug", "allegro_audio-debug", "allegro_acodec-debug", "allegro_video-debug" }

	filter { "platforms:Win32 or Win64", "configurations:Release"}
		links { "libxml2", "opengl32", "jpeg", "libpng16", "zlib", "freetype", "dumb", "FLAC", "vorbis", "vorbisfile", "ogg", "opus", "opusfile", "physfs", "theoradec", "allegro_monolith", "winmm", "Shlwapi", "psapi", "dsound" }

project "starlight"
	kind "StaticLib"
	language "C++"
	location "build/starlight"
	defines { "ALLEGRO_STATICLINK", "LIBXML_STATIC", "_LIB" }
	files { "starlight/source/**.hpp", "starlight/source/**.cpp", "starlight/source/**.h", "starlight/source/**.c" }

project "sandbox"
	kind "ConsoleApp"
	language "C++"
	location "build/sandbox"
	defines { "ALLEGRO_STATICLINK", "LIBXML_STATIC" }
	links { "starlight" }
	dependson { "starlight" }
	files { "sandbox/source/*.hpp", "sandbox/source/*.cpp" }