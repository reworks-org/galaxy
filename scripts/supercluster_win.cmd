xcopy %1deps\mimalloc\bin\%3\*.dll %2\bin\%3 /e /y /i /c
xcopy %1tools\bfxr %2\bin\%3\editor\tools\bfxr /e /y /i /c
xcopy %1tools\tiled %2\bin\%3\editor\tools\tiled /e /y /i /c
exit 0
