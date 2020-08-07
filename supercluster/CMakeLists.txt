project(supercluster CXX)

set(
supercluster_src
"src/layers/ECSLayer.cpp"
"src/layers/ECSLayer.hpp"
"src/layers/GUILayer.cpp"
"src/layers/GUILayer.hpp"
"src/layers/ImGuiLayer.cpp"
"src/layers/ImGuiLayer.hpp"
"src/main.cpp"
"src/states/Editor.cpp"
"src/states/Editor.hpp"
)

source_group(TREE "" FILES ${supercluster_src})
add_library("supercluster" STATIC ${supercluster_src})
