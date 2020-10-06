#version 450 core

in vec2 io_texels;
in float io_opacity;

out vec4 io_frag_colour;

uniform sampler2D glyph;

void main()
{
	vec4 tex = vec4(1.0, 1.0, 1.0, texture(glyph, io_texels).r);
	tex.a *= io_opacity;

	io_frag_colour = tex;
}