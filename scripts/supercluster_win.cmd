robocopy %1bin\%3 %2\bin\%3 /mt /e
robocopy %1tools\bfxr %2\bin\%3\editor\tools\bfxr /mt /e
robocopy %1tools\tiled %2\bin\%3\editor\tools\tiled /mt /e
exit 0
