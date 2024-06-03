find ../galaxy/src/ -iname *.cpp | xargs clang-format-16 -i -style=file
find ../galaxy/src/ -iname *.hpp | xargs clang-format-16 -i -style=file

find ../sandbox/src/ -iname *.cpp | xargs clang-format-16 -i -style=file

find ../supercluster/src/ -iname *.cpp | xargs clang-format-16 -i -style=file
find ../supercluster/src/ -iname *.hpp | xargs clang-format-16 -i -style=file