cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=%2 -DGALAXY_BUILD_TESTS=OFF -B%1build -H%1
msbuild %1build\dependencies\glfw3\GLFW.sln /p:Configuration=%2
msbuild %1build\dependencies\openal\OpenAL.sln /p:Configuration=%2
msbuild %1build\dependencies\zlib-ng\zlib.sln /p:Configuration=%2
robocopy %1build\dependencies\zlib-ng %1dependencies\zconf zconf.h /mt /e
exit 0