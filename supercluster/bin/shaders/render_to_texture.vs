#version 450 core
layout(location = 0) in vec2 l_pos;
layout(location = 1) in vec2 l_texels;
layout(location = 2) in float l_opacity;

out vec2 io_texels;

uniform mat4 u_projection;
uniform mat4 u_transform;
uniform float u_width;
uniform float u_height;

void main()
{
	gl_Position = u_projection * u_transform * vec4(l_pos, 0.0, 1.0);
	io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));
}