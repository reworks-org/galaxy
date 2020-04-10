#version 330 core
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec2 i_tex_coord;

out vec2 tex_coords;

uniform mat4 u_transform;
uniform mat4 u_projection;

void main()
{
	gl_Position = u_projection * u_transform * vec4(i_pos, 0.0, 1.0);
	tex_coords = i_tex_coord;
}