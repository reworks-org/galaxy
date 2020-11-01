#version 450 core
layout(location = 0) in vec4 l_vertex;

out vec2 io_texels;

uniform mat4 u_proj;

void main()
{
	gl_Position = u_proj * vec4(l_vertex.xy, 0.0, 1.0);
	io_texels = l_vertex.zw;
}