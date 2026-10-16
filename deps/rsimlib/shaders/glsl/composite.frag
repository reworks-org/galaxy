#version 430 core

uniform sampler2D source;
uniform sampler2D bloomTex;
uniform float intensity;

in vec2 uv;
out vec4 fragColor;

void main()
{
    vec4 base = texture(source, uv);
    vec4 bloom = texture(bloomTex, uv);
    fragColor = vec4(base.rgb + bloom.rgb * intensity, max(base.a, bloom.a));
}