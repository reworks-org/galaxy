#version 430 core
uniform sampler2D source;
uniform vec2 centre;
uniform float radius;
uniform float width;
uniform float strength;
in vec2 uv;
out vec4 fragColor;
void main() {
    vec2 direction = uv - centre;
    float distance_from_centre = length(direction);
    float ring = 1.0 - smoothstep(0.0, width, abs(distance_from_centre - radius));
    vec2 offset = distance_from_centre > 0.0001 ? normalize(direction) * ring * strength : vec2(0.0);
    fragColor = texture(source, clamp(uv - offset, 0.0, 1.0));
}