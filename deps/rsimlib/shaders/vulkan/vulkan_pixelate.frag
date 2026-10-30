#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(push_constant) uniform Pixelate {
    layout(offset = 64) vec2 resolution;
    vec2 pixelSize;
} effect;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;
void main() {
    vec2 blocks = max(effect.resolution / max(effect.pixelSize, vec2(1.0)), vec2(1.0));
    vec2 sampleUv = (floor(uv * blocks) + vec2(0.5)) / blocks;
    fragColor = texture(source, sampleUv);
}