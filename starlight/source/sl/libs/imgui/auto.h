/**
This libary implements the ImGui::Auto() template function.

Note: Any STL container libary should be included before this, so this libary can detect it and define functions for them.
For details see auto/auto.cpp.
*/
#pragma once
#include <string>

#ifndef IMGUI_AUTO_TREE_MAX_ELEMENT_SIZE
#define IMGUI_AUTO_TREE_MAX_ELEMENT_SIZE sizeof(std::string) //larger values generate less tree nodes
#endif
#ifndef IMGUI_AUTO_TREE_MAX_TUPLE_ELEMENTS
#define IMGUI_AUTO_TREE_MAX_TUPLE_ELEMENTS 3				 //larger values generate less tree nodes
#endif

#include "auto/core.h"
#include "auto/impl_base.h"
//#include "auto/impl_glm.h" // Comment out if you dont have GLM