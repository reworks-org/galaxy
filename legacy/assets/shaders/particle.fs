#version 450 core

in vec2 io_texels;
in float io_opacity;

out vec4 io_frag_colour;

uniform sampler2D u_texture;

void main()
{
	vec4 tex = texture(u_texture, io_texels);
	tex.a *= io_opacity;

	io_frag_colour = tex;
}