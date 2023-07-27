#pragma once

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT

#define NK_MAX_VERTEX_BUFFER  512 * 1024
#define NK_MAX_ELEMENT_BUFFER 128 * 1024

#include "nuklear.h"
#include "nuklear_glfw_gl4.h"