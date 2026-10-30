#version 450
layout(set = 0, binding = 0) uniform sampler2D uTexture;
layout(location = 0) in vec2 vTexCoord;
layout(location = 1) in vec4 vColor;
layout(location = 0) out vec4 fragColor;
void main() 
{ 
    vec4 texel = texture(uTexture, vTexCoord); 
    vec3 colour = texel.a > 0.001 ? texel.rgb / texel.a : vec3(0.0); 
    if (vColor.a < 0.0)
    {
        vec2 v = vec2(colour.r * 2.0 - 1.0, -(colour.g * 2.0 - 1.0));
        float det = vColor.b > 0.5 ? 1.0 : -1.0;
        vec2 column0 = vColor.rg * 2.0 - 1.0;
        vec2 rotated = v.x * column0 + v.y * det * vec2(-column0.y, column0.x);
        fragColor = vec4(vec3(rotated.x, -rotated.y, colour.b * 2.0 - 1.0) * 0.5 + 0.5, texel.a);
        return;
    }
    fragColor = vec4(colour, texel.a) * vColor; 
}