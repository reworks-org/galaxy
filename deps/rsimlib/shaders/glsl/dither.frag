#version 430 core
uniform sampler2D source;
uniform vec2 resolution;
uniform float pixelSize;
uniform float levels;
in vec2 uv;
out vec4 fragColor;

const float bayer4x4[16] = float[16](
     0.0,  8.0,  2.0, 10.0,
    12.0,  4.0, 14.0,  6.0,
     3.0, 11.0,  1.0,  9.0,
    15.0,  7.0, 13.0,  5.0
);

void main() {
    vec2 pixel = max(vec2(pixelSize), vec2(1.0));
    vec2 blockUv = floor(uv * resolution / pixel);
    ivec2 cell = ivec2(mod(blockUv, 4.0));
    float threshold = (bayer4x4[cell.y * 4 + cell.x] + 0.5) / 16.0;
    vec4 color = texture(source, uv);
    float steps = max(levels - 1.0, 1.0);
    vec3 adjusted = color.rgb + (threshold - 0.5) / steps;
    vec3 quantised = floor(adjusted * steps + 0.5) / steps;
    fragColor = vec4(clamp(quantised, 0.0, 1.0), color.a);
}
