robocopy %1tools\ldtk %2\bin\%3\editor\tools\ldtk /mt /e
robocopy %1tools\bfxr %2\bin\%3\editor\tools\bfxr /mt /e
%1tools\7zip\7za.exe e -aos %2\bin\%3\editor\tools\ldtk\LDtk.7z -o%2\bin\%3\editor\tools\ldtk\
exit 0
