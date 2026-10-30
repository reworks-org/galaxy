#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(push_constant) uniform ChromaticAberration { layout(offset = 64) float strength; } effect;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;
void main() {
    vec2 offset = (uv - vec2(0.5)) * effect.strength;
    float red = texture(source, uv + offset).r;
    float green = texture(source, uv).g;
    float blue = texture(source, uv - offset).b;
    fragColor = vec4(red, green, blue, texture(source, uv).a);
}