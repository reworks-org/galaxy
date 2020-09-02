#version 450 core
layout (location = 0) in vec2 l_pos;
layout (location = 1) in vec2 l_texels;
layout (location = 2) in vec2 l_offset;

out vec4 io_colour;

void main()
{
    gl_Position = vec4(l_pos + l_offset, 0.0, 1.0);
    io_colour = vec2(l_texels, 0.0, 1.0);
}  