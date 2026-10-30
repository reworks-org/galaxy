#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(push_constant) uniform HeatHaze {
    layout(offset = 64) float strength;
    float frequency;
    float time;
} effect;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;
void main() {
    vec2 wave = vec2(sin(uv.y * effect.frequency + effect.time) * effect.strength,
                     cos(uv.x * effect.frequency * 1.17 + effect.time * 1.21) * effect.strength);
    fragColor = texture(source, uv + wave);
}