#version 450
layout(set = 0, binding = 0) uniform sampler2D source;
layout(push_constant) uniform Blur { layout(offset = 64) vec2 texel; vec2 direction; float radius; int premultipliedSource; float opacity; int premultipliedOutput; } blur;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;
void main() {
    float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    vec4 texelSample = texture(source, uv);
    vec4 result = vec4(blur.premultipliedSource != 0 ? texelSample.rgb : texelSample.rgb * texelSample.a, texelSample.a) * weights[0];
    for (int i = 1; i < 5; ++i) {
        vec2 offset = blur.direction * blur.texel * blur.radius * float(i);
        texelSample = texture(source, uv + offset);
        result += vec4(blur.premultipliedSource != 0 ? texelSample.rgb : texelSample.rgb * texelSample.a, texelSample.a) * weights[i];
        texelSample = texture(source, uv - offset);
        result += vec4(blur.premultipliedSource != 0 ? texelSample.rgb : texelSample.rgb * texelSample.a, texelSample.a) * weights[i];
    }
    fragColor = blur.premultipliedOutput != 0
        ? vec4(result.rgb * blur.opacity, result.a * blur.opacity)
        : vec4(result.a > 0.001 ? result.rgb / result.a : vec3(0.0), result.a * blur.opacity);
}
