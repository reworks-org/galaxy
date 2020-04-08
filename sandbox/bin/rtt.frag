#version 330 core

in vec2 tex_coords;

out vec4 frag_colour;

uniform sampler2D u_render_texture;

void main()
{
	frag_colour = texture(u_render_texture, tex_coords);
}