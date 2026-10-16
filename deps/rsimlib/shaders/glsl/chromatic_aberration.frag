#version 430 core
uniform sampler2D source;
uniform float strength;
in vec2 uv;
out vec4 fragColor;
void main() {
    vec2 offset = (uv - vec2(0.5)) * strength;
    float red = texture(source, uv + offset).r;
    float green = texture(source, uv).g;
    float blue = texture(source, uv - offset).b;
    fragColor = vec4(red, green, blue, texture(source, uv).a);
}