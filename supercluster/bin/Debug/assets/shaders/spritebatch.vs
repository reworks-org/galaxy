#version 450 core
layout(location = 0) in vec2 l_pos;
layout(location = 1) in vec2 l_texels;
layout(location = 2) in float l_opacity;

out vec2 io_texels;
out vec4 io_colour;

uniform mat4 u_cameraView;
uniform mat4 u_cameraProj;
uniform float u_width;
uniform float u_height;

void main()
{
	mat4 camera = u_cameraProj * u_cameraView;
	gl_Position =  camera * vec4(l_pos, 0.0, 1.0);

	io_colour = vec4(0.0, 0.0, 0.0, l_opacity);
	io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));
}