#version 330 core
out vec4 fragColour;

in vec2 texCoords;

uniform sampler2D renderTexture;

void main()
{
	fragColour = texture(renderTexture, texCoords);
}