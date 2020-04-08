#version 330 core
layout(location = 0) in vec2 l_pos;
layout(location = 1) in vec2 l_tex_coords;

out vec2 tex_coords;

uniform mat4 u_projection;
uniform mat4 u_transform;

void main()
{
	gl_Position = u_projection * u_transform * vec4(l_pos, 0.0, 1.0);
	tex_coords = l_tex_coords;
}