#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(push_constant) uniform CRT {
    layout(offset = 64) vec2 resolution;
    float pixelSize;
    float scanlineStrength;
    float curvature;
} effect;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;

void main() {
    vec2 centered = uv - vec2(0.5);
    centered *= 1.0 + effect.curvature * dot(centered, centered);
    vec2 warpedUv = centered + vec2(0.5);
    vec2 safeResolution = max(effect.resolution, vec2(1.0));
    vec2 pixel = max(vec2(effect.pixelSize), vec2(1.0));
    vec2 blockSize = safeResolution / pixel;
    vec2 blockUv = floor(warpedUv * blockSize) / blockSize;
    vec4 color = texture(source, clamp(blockUv, vec2(0.0), vec2(1.0)));
    float scan = 0.5 + 0.5 * sin((warpedUv.y * safeResolution.y) * 3.14159265);
    color.rgb *= 0.75 + effect.scanlineStrength * (0.25 + 0.75 * scan);
    float vignette = smoothstep(1.2, 0.2, length(centered * vec2(1.25, 1.0)));
    color.rgb *= vignette;
    fragColor = color;
}
