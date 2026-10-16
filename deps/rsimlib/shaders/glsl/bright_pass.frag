#version 430 core

uniform sampler2D source;
uniform float threshold;

in vec2 uv;
out vec4 fragColor;

void main()
{
    vec3 colour = max(texture(source, uv).rgb - vec3(threshold), vec3(0.0));
    float alpha = clamp(max(colour.r, max(colour.g, colour.b)), 0.0, 1.0);
    fragColor = vec4(colour, alpha);
}