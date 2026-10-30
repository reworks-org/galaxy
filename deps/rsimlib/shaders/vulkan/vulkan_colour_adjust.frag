#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(push_constant) uniform ColourAdjust {
    layout(offset = 64) float brightness;
    float contrast;
    float saturation;
    float exposure;
} adjust;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;
void main() {
    vec4 colour = texture(source, uv);
    vec3 adjusted = colour.rgb + vec3(adjust.brightness);
    adjusted = (adjusted - 0.5) * adjust.contrast + 0.5;
    float luminance = dot(adjusted, vec3(0.2126, 0.7152, 0.0722));
    adjusted = mix(vec3(luminance), adjusted, adjust.saturation);
    adjusted *= pow(2.0, adjust.exposure);
    fragColor = vec4(adjusted, colour.a);
}
