#version 430 core
uniform sampler2D source;
uniform float brightness;
uniform float contrast;
uniform float saturation;
uniform float exposure;
in vec2 uv;
out vec4 fragColor;
void main() {
    vec4 colour = texture(source, uv);
    vec3 adjusted = colour.rgb + vec3(brightness);
    adjusted = (adjusted - 0.5) * contrast + 0.5;
    float luminance = dot(adjusted, vec3(0.2126, 0.7152, 0.0722));
    adjusted = mix(vec3(luminance), adjusted, saturation);
    adjusted *= pow(2.0, exposure);
    fragColor = vec4(adjusted, colour.a);
}