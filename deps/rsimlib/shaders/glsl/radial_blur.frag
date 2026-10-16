#version 430 core
uniform sampler2D source;
uniform vec2 centre;
uniform float strength;
uniform int samples;
in vec2 uv;
out vec4 fragColor;
void main() {
    vec2 delta = (centre - uv) * strength;
    vec4 result = texture(source, uv);
    int count = clamp(samples, 1, 16);
    for (int i = 1; i <= count; ++i)
        result += texture(source, uv + delta * (float(i) / float(count)));
    fragColor = result / float(count + 1);
}