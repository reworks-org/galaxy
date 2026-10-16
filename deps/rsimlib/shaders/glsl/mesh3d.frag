#version 430 core

in vec3 vNormal;
in vec3 vColour;
in vec2 vUv;

uniform sampler2D uTexture;

// xyz: unit vector toward the light, w: ambient. Unlit draws use (0, 0, 0, 1).
uniform vec4 uLight;

out vec4 fragColor;

void main()
{
    float diffuse = max(dot(normalize(vNormal), uLight.xyz), 0.0);
    float brightness = uLight.w + (1.0 - uLight.w) * diffuse;
    fragColor = vec4(vColour * texture(uTexture, vUv).rgb * brightness, 1.0);
}
