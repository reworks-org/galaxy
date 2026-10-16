#version 430 core
uniform sampler2D source;
uniform vec2 resolution;
uniform float pixelSize;
in vec2 uv;
out vec4 fragColor;
void main() {
    vec2 blocks = max(resolution / max(pixelSize, 1.0), vec2(1.0));
    vec2 sampleUv = (floor(uv * blocks) + vec2(0.5)) / blocks;
    fragColor = texture(source, sampleUv);
}