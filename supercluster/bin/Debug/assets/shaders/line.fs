#version 450 core

in vec4 io_colour;

out vec4 io_frag_colour;

void main()
{
	io_frag_colour = io_colour;
}