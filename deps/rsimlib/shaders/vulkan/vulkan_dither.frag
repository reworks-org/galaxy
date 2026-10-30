#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(push_constant) uniform Dither {
    layout(offset = 64) vec2 resolution;
    float pixelSize;
    float levels;
} effect;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;

const float bayer4x4[16] = float[16](
     0.0,  8.0,  2.0, 10.0,
    12.0,  4.0, 14.0,  6.0,
     3.0, 11.0,  1.0,  9.0,
    15.0,  7.0, 13.0,  5.0
);

void main() {
    vec2 pixel = max(vec2(effect.pixelSize), vec2(1.0));
    vec2 blockUv = floor(uv * effect.resolution / pixel);
    ivec2 cell = ivec2(mod(blockUv, 4.0));
    float threshold = (bayer4x4[cell.y * 4 + cell.x] + 0.5) / 16.0;
    vec4 color = texture(source, uv);
    float steps = max(effect.levels - 1.0, 1.0);
    vec3 adjusted = color.rgb + (threshold - 0.5) / steps;
    vec3 quantised = floor(adjusted * steps + 0.5) / steps;
    fragColor = vec4(clamp(quantised, 0.0, 1.0), color.a);
}
