#version 430 core

uniform sampler2D source;
uniform float radius;
uniform float softness;
uniform float intensity;

in vec2 uv;
out vec4 fragColor;

void main()
{
    vec4 base = texture(source, uv);
    vec2 centred = uv - vec2(0.5);
    centred.x *= float(textureSize(source, 0).x) / float(textureSize(source, 0).y);

    float dist = length(centred);
    float inner = max(radius - softness, 0.0);
    float t = clamp((dist - inner) / max(softness, 0.0001), 0.0, 1.0);

    fragColor = vec4(base.rgb * (1.0 - t * intensity), base.a);
}