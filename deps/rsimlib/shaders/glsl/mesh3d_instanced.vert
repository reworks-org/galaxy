#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColour;
layout(location = 3) in vec2 aUv;
layout(location = 4) in vec4 aTangent;
// Per-instance model matrix (columns) and tint.
layout(location = 6) in vec4 aModel0;
layout(location = 7) in vec4 aModel1;
layout(location = 8) in vec4 aModel2;
layout(location = 9) in vec4 aModel3;
layout(location = 10) in vec4 aInstanceTint;

// The view-projection; each instance's model matrix is applied here.
uniform mat4 uMvp;
// Only w is used: the material tint colour.
uniform vec4 uNormalRows[3];

out vec3 vNormal;
out vec4 vTangent;
out vec4 vColour;
out vec2 vUv;
out float vTerrain;
out float vViewDepth;

void main()
{
    mat4 model = mat4(aModel0, aModel1, aModel2, aModel3);
    gl_Position = uMvp * (model * vec4(aPosition, 1.0));
    vViewDepth = gl_Position.w;
    // The inverse transpose of the model's 3x3 is its cofactor matrix over the determinant; the fragment shader
    // normalises, so only the determinant's sign (mirrored instances) is needed.
    vec3 c0 = aModel0.xyz;
    vec3 c1 = aModel1.xyz;
    vec3 c2 = aModel2.xyz;
    mat3 cofactor = mat3(cross(c1, c2), cross(c2, c0), cross(c0, c1));
    float handedness = dot(c0, cross(c1, c2)) < 0.0 ? -1.0 : 1.0;
    vNormal = cofactor * aNormal * handedness;
    vTangent = vec4(cofactor * aTangent.xyz * handedness, aTangent.w);
    vUv = aUv;
    vColour = vec4(aColour.rgb * vec3(uNormalRows[0].w, uNormalRows[1].w, uNormalRows[2].w) * aInstanceTint.rgb,
                   aColour.a * aInstanceTint.a);
    vTerrain = -1.0;
}
