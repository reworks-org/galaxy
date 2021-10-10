robocopy ../ ./ .clang-format
for /r %%t in (*.cpp *.hpp) do clang-format -i -style=file "%%t"
del .clang-format