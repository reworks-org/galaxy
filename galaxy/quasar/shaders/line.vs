#version 450 core
layout(location = 0) in vec2 l_pos;
layout(location = 1) in vec4 l_colour;

out vec4 io_colour;

uniform mat4 u_cameraProj;
uniform mat4 u_cameraView;
uniform mat4 u_transform;

void main()
{
	gl_Position = u_cameraProj * u_cameraView * u_transform * vec4(l_pos, 0.0, 1.0);
	io_colour   = l_colour;
}