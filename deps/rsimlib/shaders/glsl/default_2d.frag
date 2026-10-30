#version 430 core

in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTexture;

out vec4 fragColor;

void main()
{
    vec4 texel = texture(uTexture, vTexCoord);
    if (vColor.a < 0.0)
    {
        // Normal pass: vColor carries an orthogonal 2x2 transform (cos, sin, determinant sign) for the normal's xy.
        vec2 v = vec2(texel.r * 2.0 - 1.0, -(texel.g * 2.0 - 1.0));
        float det = vColor.b > 0.5 ? 1.0 : -1.0;
        vec2 column0 = vColor.rg * 2.0 - 1.0;
        vec2 rotated = v.x * column0 + v.y * det * vec2(-column0.y, column0.x);
        fragColor = vec4(vec3(rotated.x, -rotated.y, texel.b * 2.0 - 1.0) * 0.5 + 0.5, texel.a);
        return;
    }
    fragColor = texel * vColor;
}