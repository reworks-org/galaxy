powershell -Command "Invoke-WebRequest https://github.com/premake/premake-core/releases/download/v5.0.0-alpha13/premake-5.0.0-alpha13-windows.zip -OutFile premake5.zip"
powershell -nologo -noprofile -Command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::ExtractToDirectory('premake5.zip', '.'); }"
premake5 --os=windows --verbose --file=premake5.lua vs2017