#version 450 core
layout(location = 0) in vec2 l_pos;
layout(location = 1) in vec2 l_texels;

out vec2 io_texels;
out vec2 io_tex_size;
out float io_opacity;

uniform mat4 u_cameraView;
uniform mat4 u_cameraProj;
uniform mat4 u_transform;
uniform float u_width;
uniform float u_height;
uniform float u_opacity;

void main()
{
	mat4 camera = u_cameraProj * u_cameraView;
	gl_Position =  camera * u_transform * vec4(l_pos, 0.0, 1.0);

	//io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));
    io_texels = l_texels;
    io_tex_size = vec2(u_width, u_height);
	io_opacity = u_opacity;
}