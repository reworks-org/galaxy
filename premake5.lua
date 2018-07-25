-- premake5.lua

workspace "starlight"
	configurations {"Debug32", "Debug64", "Release32", "Release64"}

	filter "configurations:Debug32"
		defines { }
		flags { }
		architecture "x32"

	filter "configurations:Debug64"
		defines { }
		flags { }
		architecture "x64"

   filter "configurations:Release32"
		defines { }
		optimize "On"
		architecture "x32"

	filter "configurations:Release64"
		defines { }
		optimize "On"
		architecture "x64"

project "starlight"
	kind ""
	language "C++"
	defines {}