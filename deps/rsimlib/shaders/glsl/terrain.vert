#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
// Weights of terrain layers 0-3, then 4-7.
layout(location = 2) in vec4 aWeightsA;
// Metres along the ground, wrapped per chunk by a whole number of tiles of every layer.
layout(location = 3) in vec2 aUv;
// Offset to the coarser level's surface.
layout(location = 4) in vec4 aTangent;
layout(location = 5) in vec4 aWeightsB;

uniform mat4 uMvp;
// [0] xyz: eye (world axes, relative to the chunk), w: morph start. [1] x: morph end, y: fade start, z: fade end,
// uTurn: quaternion (xyz, w) turning the mesh into world axes (a rotating planet).
uniform vec4 uTerrain[2];
uniform vec4 uTurn;

vec3 turnBy(vec3 v)
{
    return v + 2.0 * cross(uTurn.xyz, cross(uTurn.xyz, v) + uTurn.w * v);
}

out vec3 vNormal;
out vec3 vPosition;
out vec4 vWeightsA;
out vec4 vWeightsB;
out vec2 vUv;
// 0 near the eye, 1 where the layers have faded completely to their average colour.
out float vFade;
// View-space depth, for fog.
out float vViewDepth;

void main()
{
    vec3 eye = uTerrain[0].xyz;
    vec3 turned = turnBy(aPosition);
    float morph = smoothstep(uTerrain[0].w, uTerrain[1].x, distance(turned, eye));
    vec3 position = turned + turnBy(aTangent.xyz) * morph;
    gl_Position = uMvp * vec4(position, 1.0);
    vViewDepth = gl_Position.w;
    vNormal = turnBy(aNormal);
    vPosition = position;
    vWeightsA = aWeightsA;
    vWeightsB = aWeightsB;
    vUv = aUv;
    vFade = uTerrain[1].z > uTerrain[1].y ? smoothstep(uTerrain[1].y, uTerrain[1].z, distance(position, eye)) : 0.0;
}
