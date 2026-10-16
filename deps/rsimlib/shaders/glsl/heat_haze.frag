#version 430 core
uniform sampler2D source;
uniform float strength;
uniform float frequency;
uniform float time;
in vec2 uv;
out vec4 fragColor;
void main() {
    vec2 wave = vec2(sin(uv.y * frequency + time) * strength,
                     cos(uv.x * frequency * 1.17 + time * 1.21) * strength);
    fragColor = texture(source, uv + wave);
}