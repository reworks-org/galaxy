#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(push_constant) uniform RadialBlur {
    layout(offset = 64) vec2 centre;
    float strength;
    int samples;
} effect;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;
void main() {
    vec2 delta = (effect.centre - uv) * effect.strength;
    vec4 result = texture(source, uv);
    int count = clamp(effect.samples, 1, 16);
    for (int i = 1; i <= count; ++i)
        result += texture(source, uv + delta * (float(i) / float(count)));
    fragColor = result / float(count + 1);
}