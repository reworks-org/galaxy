REM This is used to strip .c and .cpp files from the source, leaving you with the include headers

xcopy "/REngine3/source/" "/includes/" /S/H/E/K/F/C/I
cd /includes
del /S *.c
del /S *.cpp