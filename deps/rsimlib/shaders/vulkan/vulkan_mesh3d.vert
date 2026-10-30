#version 450
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColour;
layout(location = 3) in vec2 aUv;
layout(location = 4) in vec4 aTangent;
layout(location = 5) in vec4 aExtra;
layout(push_constant) uniform Transform { mat4 mvp; vec4 normalRows[3]; } transform;
layout(location = 0) out vec3 vNormal;
layout(location = 1) out vec4 vColour;
layout(location = 2) out vec2 vUv;
layout(location = 3) out vec4 vTangent;
layout(location = 4) out float vTerrain;
layout(location = 5) out float vViewDepth;
void main()
{
    if (transform.normalRows[0].w < 0.0)
    {
        vec3 eye = transform.normalRows[1].xyz;
        float morph = smoothstep(transform.normalRows[1].w, transform.normalRows[2].x, distance(aPosition, eye));
        vec3 position = aPosition + aTangent.xyz * morph;
        gl_Position = transform.mvp * vec4(position, 1.0);
        vViewDepth = gl_Position.w;
        vNormal = aNormal;
        vTangent = vec4(aTangent.xyz, 1.0);
        vColour = aColour;
        vUv = aUv;
        vTerrain = 0.0;
        return;
    }
    gl_Position = transform.mvp * vec4(aPosition, 1.0);
    vViewDepth = gl_Position.w;
    vNormal = vec3(dot(transform.normalRows[0].xyz, aNormal),
                   dot(transform.normalRows[1].xyz, aNormal),
                   dot(transform.normalRows[2].xyz, aNormal));
    vTangent = vec4(dot(transform.normalRows[0].xyz, aTangent.xyz),
                    dot(transform.normalRows[1].xyz, aTangent.xyz),
                    dot(transform.normalRows[2].xyz, aTangent.xyz), aTangent.w);
    vUv = aUv;
    vColour = vec4(aColour.rgb * vec3(transform.normalRows[0].w, transform.normalRows[1].w, transform.normalRows[2].w), aColour.a);
    vTerrain = -1.0;
}
