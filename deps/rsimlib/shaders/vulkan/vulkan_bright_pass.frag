#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(push_constant) uniform Bright { layout(offset = 64) float threshold; } bright;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;
void main() {
    vec3 colour = max(texture(source, uv).rgb - vec3(bright.threshold), vec3(0.0));
    float alpha = clamp(max(colour.r, max(colour.g, colour.b)), 0.0, 1.0);
    fragColor = vec4(colour, alpha);
}
