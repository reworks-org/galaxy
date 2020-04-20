#version 450 core

in vec2 io_texels;
in vec4 io_colour;

out vec4 io_frag_colour;

uniform sampler2D u_texture;

void main()
{
	io_frag_colour = texture(u_texture, io_texels);// * io_colour;
}