xcopy %1supercluster\bin\Release\editor\export\*.* %1output\dist\ /e /y /i /c
xcopy %1output\galaxy\Release\galaxy.lib  %1output\dist\  /e /y /i /c
exit 0
