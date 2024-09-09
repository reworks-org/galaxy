robocopy %1bin\%3\*.dll %1output\dist /mt /e
xcopy %1supercluster\bin\%3\editor\export\*.* %1output\dist\ /e /y /i /c
xcopy %1output\galaxy\%3\galaxy.lib  %1output\dist\  /e /y /i /c
exit 0
