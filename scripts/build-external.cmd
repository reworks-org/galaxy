cmake -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=%2 -DGALAXY_BUILD_TESTS=OFF -B%1build -H%1
msbuild %1build\dependencies\efsw\efsw.sln
msbuild %1build\dependencies\glfw3\GLFW.sln
msbuild %1build\dependencies\openal\OpenAL.sln
msbuild %1build\dependencies\zlib-ng\zlib.sln
robocopy %1build\dependencies\zlib-ng %1dependencies\zconf zconf.h /mt /e
exit 0