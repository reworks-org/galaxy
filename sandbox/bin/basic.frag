#version 330 core

in vec2 tex_coords;

out vec4 frag_colour;

uniform sampler2D u_texture;
uniform vec4 u_tint;

void main()
{
	frag_colour = texture(u_texture, tex_coords);
}