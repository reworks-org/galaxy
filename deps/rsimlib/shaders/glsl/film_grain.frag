#version 430 core
uniform sampler2D source;
uniform float strength;
uniform float time;
in vec2 uv;
out vec4 fragColor;

float noise(uvec3 value) {
    uint hash = value.x * 0x1f123bb5u + value.y * 0x159a55e5u + value.z * 0x3c6ef372u;
    hash ^= hash >> 16u;
    hash *= 0x45d9f3bu;
    hash ^= hash >> 16u;
    return float(hash & 0x00ffffffu) / 16777215.0;
}

void main() {
    vec4 colour = texture(source, uv);
    uvec2 pixel = uvec2(floor(uv * vec2(800.0, 600.0)));
    uint frame = uint(floor(time * 60.0));
    float grain = noise(uvec3(pixel, frame)) * 2.0 - 1.0;
    fragColor = vec4(clamp(colour.rgb + grain * strength, 0.0, 1.0), colour.a);
}