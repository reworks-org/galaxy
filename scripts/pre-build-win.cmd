cmake -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=%2 -DGALAXY_BUILD_TESTS=OFF -DGALAXY_ENABLE_DOXYGEN=OFF -B%1build -H%1
robocopy %1build\dependencies\zlib-ng %1dependencies\zconf zconf.h /mt /e
exit 0