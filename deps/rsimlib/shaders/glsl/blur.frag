#version 430 core

uniform sampler2D source;
uniform vec2 texel;
uniform vec2 direction;
uniform float radius;
uniform int premultipliedSource;
uniform float opacity;
uniform int premultipliedOutput;

in vec2 uv;
out vec4 fragColor;

void main()
{
    float weights[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
    vec4 texelSample = texture(source, uv);
    vec4 result = vec4(premultipliedSource != 0 ? texelSample.rgb : texelSample.rgb * texelSample.a, texelSample.a) * weights[0];

    for (int i = 1; i < 5; ++i)
    {
        vec2 offset = direction * texel * radius * float(i);

        texelSample = texture(source, uv + offset);
        result += vec4(premultipliedSource != 0 ? texelSample.rgb : texelSample.rgb * texelSample.a, texelSample.a) * weights[i];

        texelSample = texture(source, uv - offset);
        result += vec4(premultipliedSource != 0 ? texelSample.rgb : texelSample.rgb * texelSample.a, texelSample.a) * weights[i];
    }

    fragColor = premultipliedOutput != 0
        ? vec4(result.rgb * opacity, result.a * opacity)
        : vec4(result.a > 0.001 ? result.rgb / result.a : vec3(0.0), result.a * opacity);
}