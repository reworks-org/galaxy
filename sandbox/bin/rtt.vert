#version 330 core
layout(location = 0) in vec2 l_pos;
layout(location = 1) in vec2 l_texCoords;

out vec2 texCoords;

uniform mat4 u_transform;

void main()
{
	gl_Position = u_transform * vec4(l_pos.x, l_pos.y, 0.0, 1.0);
	texCoords = l_texCoords;
}