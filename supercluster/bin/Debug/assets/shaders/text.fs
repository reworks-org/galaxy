#version 450 core

in vec2 io_texels;
in float io_opacity;
out vec4 io_frag_colour;

uniform vec4 u_colour;
uniform sampler2D u_texture;

void main()
{
	io_frag_colour = texture(u_texture, io_texels) * u_colour;
	io_frag_colour.a = io_opacity;
}