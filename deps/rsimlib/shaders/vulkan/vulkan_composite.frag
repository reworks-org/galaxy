#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(set = 0, binding = 1) uniform sampler2D bloomTex;
layout(push_constant) uniform Composite { layout(offset = 64) float intensity; } composite;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;
void main() {
    vec4 base = texture(source, uv);
    vec4 bloom = texture(bloomTex, uv);
    fragColor = vec4(base.rgb + bloom.rgb * composite.intensity, max(base.a, bloom.a));
}
