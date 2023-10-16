robocopy %1tools\LDtk %2\bin\%3\editor_data\tools\LDtk /mt /e
robocopy %1tools\LDtk\settings %AppData%\LDtk\settings /mt /e
robocopy %1tools\bfxr %2\bin\%3\editor_data\tools\bfxr /mt /e
%1tools\7zip\7za.exe e %2\bin\%3\editor_data\tools\LDtk\LDtk.7z -o%2\bin\%3\editor_data\tools\LDtk\
exit 0