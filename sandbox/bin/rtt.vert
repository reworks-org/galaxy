#version 450 core
layout(location = 0) in vec2 l_pos;
layout(location = 1) in vec4 l_colour;
layout(location = 2) in vec2 l_texels;

out vec2 io_texels;

uniform mat4 u_projection;
uniform float u_width;
uniform float u_height;

void main()
{
	gl_Position = u_projection * vec4(l_pos, 0.0, 1.0);
	io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));
}