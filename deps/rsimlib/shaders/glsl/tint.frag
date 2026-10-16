#version 430 core

in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTexture;

out vec4 fragColor;

void main()
{
    fragColor = texture(uTexture, vTexCoord) * vColor * vec4(1.0, 0.65, 0.35, 1.0);
}
