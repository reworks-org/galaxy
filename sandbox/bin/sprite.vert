#version 450 core
layout(location = 0) in vec2 l_pos;
layout(location = 1) in vec4 l_colour;
layout(location = 2) in vec2 l_texels;

out vec4 io_colour;
out vec2 io_texels;

uniform mat4 u_cameraView;
uniform mat4 u_cameraProj;

void main()
{
	mat4 camera = u_cameraProj * u_cameraView;
	gl_Position =  camera * vec4(l_pos, 0.0, 1.0);
	//io_colour = l_colour;
	io_texels = l_texels;
}