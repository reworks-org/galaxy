xcopy %1out\mimalloc\%3\*.dll %2\bin\%3 /e /y /i /c
xcopy %1supercluster\bin\%3\editor\export\*.* %1out\dist\ /e /y /i /c
xcopy %1out\galaxy\%3\galaxy.lib  %1out\dist\  /e /y /i /c
exit 0
