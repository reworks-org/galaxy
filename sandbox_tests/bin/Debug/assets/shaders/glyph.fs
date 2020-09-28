#version 450 core

in float io_opacity;
in vec2 io_texels;

out vec4 io_frag_colour;

uniform sampler2D glyph;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(glyph, io_texels).r);
	io_frag_colour.a = sampled.a * io_opacity;
}