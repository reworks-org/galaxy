#version 430 core

in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTexture;

out vec4 fragColor;

void main()
{
    fragColor = texture(uTexture, vTexCoord) * vColor;
}