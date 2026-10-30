#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(push_constant) uniform Shockwave {
    layout(offset = 64) vec2 centre;
    float radius;
    float width;
    float strength;
} effect;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;
void main() {
    vec2 direction = uv - effect.centre;
    float distance_from_centre = length(direction);
    float ring = 1.0 - smoothstep(0.0, effect.width, abs(distance_from_centre - effect.radius));
    vec2 offset = distance_from_centre > 0.0001 ? normalize(direction) * ring * effect.strength : vec2(0.0);
    fragColor = texture(source, clamp(uv - offset, 0.0, 1.0));
}