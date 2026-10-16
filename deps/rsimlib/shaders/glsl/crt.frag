#version 430 core
uniform sampler2D source;
uniform vec2 resolution;
uniform float pixelSize;
uniform float scanlineStrength;
uniform float curvature;
in vec2 uv;
out vec4 fragColor;

void main() {
    vec2 centered = uv - vec2(0.5);
    centered *= 1.0 + curvature * dot(centered, centered);
    vec2 warpedUv = centered + vec2(0.5);
    vec2 texel = 1.0 / max(resolution, vec2(1.0));
    vec2 blockUv = floor(warpedUv * (resolution / max(pixelSize, 1.0))) / max((resolution / max(pixelSize, 1.0)), vec2(1.0));
    vec4 color = texture(source, clamp(blockUv, vec2(0.0), vec2(1.0)));
    float scan = 0.5 + 0.5 * sin((warpedUv.y * resolution.y) * 3.14159265);
    color.rgb *= 0.75 + scanlineStrength * (0.25 + 0.75 * scan);
    float vignette = smoothstep(1.2, 0.2, length(centered * vec2(1.25, 1.0)));
    color.rgb *= vignette;
    fragColor = color;
}
