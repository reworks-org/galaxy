robocopy %1tools\LDtk %2\bin\%3\editor\tools\LDtk /mt /e
robocopy %1tools\bfxr %2\bin\%3\editor\tools\bfxr /mt /e
%1tools\7zip\7za.exe e -aos %2\bin\%3\editor\tools\LDtk\LDtk.7z -o%2\bin\%3\editor\tools\LDtk\
exit 0