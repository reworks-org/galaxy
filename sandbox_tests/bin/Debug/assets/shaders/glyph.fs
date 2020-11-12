#version 450 core

in vec2 io_texels;

out vec4 io_frag_colour;

uniform vec3 u_colour;

uniform sampler2D u_text;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_text, io_texels).r);
    io_frag_colour = vec4(u_colour, 1.0) * sampled;
}