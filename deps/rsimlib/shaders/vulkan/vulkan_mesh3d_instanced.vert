#version 450
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColour;
layout(location = 3) in vec2 aUv;
layout(location = 4) in vec4 aTangent;
layout(location = 5) in vec4 aExtra;
// Per-instance model matrix (columns) and tint.
layout(location = 6) in vec4 aModel0;
layout(location = 7) in vec4 aModel1;
layout(location = 8) in vec4 aModel2;
layout(location = 9) in vec4 aModel3;
layout(location = 10) in vec4 aInstanceTint;
// mvp is the view-projection; only the w of each normal row (the material tint) is used.
layout(push_constant) uniform Transform { mat4 mvp; vec4 normalRows[3]; } transform;
layout(location = 0) out vec3 vNormal;
layout(location = 1) out vec4 vColour;
layout(location = 2) out vec2 vUv;
layout(location = 3) out vec4 vTangent;
layout(location = 4) out float vTerrain;
layout(location = 5) out float vViewDepth;
void main()
{
    mat4 model = mat4(aModel0, aModel1, aModel2, aModel3);
    gl_Position = transform.mvp * (model * vec4(aPosition, 1.0));
    vViewDepth = gl_Position.w;
    vec3 c0 = aModel0.xyz;
    vec3 c1 = aModel1.xyz;
    vec3 c2 = aModel2.xyz;
    mat3 cofactor = mat3(cross(c1, c2), cross(c2, c0), cross(c0, c1));
    float handedness = dot(c0, cross(c1, c2)) < 0.0 ? -1.0 : 1.0;
    vNormal = cofactor * aNormal * handedness;
    vTangent = vec4(cofactor * aTangent.xyz * handedness, aTangent.w);
    vUv = aUv;
    vColour = vec4(aColour.rgb * vec3(transform.normalRows[0].w, transform.normalRows[1].w, transform.normalRows[2].w) * aInstanceTint.rgb,
                   aColour.a * aInstanceTint.a);
    vTerrain = -1.0;
}
