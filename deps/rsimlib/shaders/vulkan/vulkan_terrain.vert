#version 450
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aWeightsA;
layout(location = 3) in vec2 aUv;
layout(location = 4) in vec4 aTangent;
layout(location = 5) in vec4 aWeightsB;
layout(push_constant) uniform Transform { mat4 mvp; vec4 normalRows[3]; } transform;
layout(location = 0) out vec3 vNormal;
layout(location = 1) out vec3 vPosition;
layout(location = 2) out vec4 vWeightsA;
layout(location = 3) out vec4 vWeightsB;
layout(location = 4) out vec2 vUv;
layout(location = 5) out float vFade;
layout(location = 6) out float vViewDepth;
void main()
{
    vec3 eye = transform.normalRows[1].xyz;
    vec4 q = vec4(transform.normalRows[0].xyz, transform.normalRows[2].w);
    vec3 turned = aPosition + 2.0 * cross(q.xyz, cross(q.xyz, aPosition) + q.w * aPosition);
    float morph = smoothstep(transform.normalRows[1].w, transform.normalRows[2].x, distance(turned, eye));
    vec3 position = turned + (aTangent.xyz + 2.0 * cross(q.xyz, cross(q.xyz, aTangent.xyz) + q.w * aTangent.xyz)) * morph;
    gl_Position = transform.mvp * vec4(position, 1.0);
    vViewDepth = gl_Position.w;
    vNormal = aNormal + 2.0 * cross(q.xyz, cross(q.xyz, aNormal) + q.w * aNormal);
    vPosition = position;
    vWeightsA = aWeightsA;
    vWeightsB = aWeightsB;
    vUv = aUv;
    vFade = transform.normalRows[2].z > transform.normalRows[2].y
        ? smoothstep(transform.normalRows[2].y, transform.normalRows[2].z, distance(position, eye)) : 0.0;
}
