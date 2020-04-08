#version 330 core
layout(location = 0) in vec2 l_pos;
layout(location = 1) in vec2 l_tex_coords;
layout(location = 2) in float l_id;

out vec2 tex_coords;

uniform mat4 u_camera;
uniform mat4 u_transform[10];

void main()
{
	highp int index = int(l_id);
	gl_Position = u_camera * u_transform[index] * vec4(l_pos, 0.0, 1.0);
	tex_coords = l_tex_coords;
}