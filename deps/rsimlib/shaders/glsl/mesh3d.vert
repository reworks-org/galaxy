#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColour;
layout(location = 3) in vec2 aUv;
layout(location = 4) in vec4 aTangent;

uniform mat4 uMvp;
// Rows of the normal matrix; w holds the tint colour. A negative uNormalRows[0].w selects untextured terrain, where
// the rows carry: [1] xyz: eye (mesh space), w: morph start. [2] x: morph end.
uniform vec4 uNormalRows[3];

out vec3 vNormal;
out vec4 vTangent;
out vec4 vColour;
out vec2 vUv;
// -1 for ordinary meshes, 0 for terrain, which is coloured by its vertex colour alone.
out float vTerrain;
// View-space depth, for fog.
out float vViewDepth;

void main()
{
    if (uNormalRows[0].w < 0.0)
    {
        // The tangent attribute holds the offset to the coarser level's surface.
        vec3 eye = uNormalRows[1].xyz;
        float morph = smoothstep(uNormalRows[1].w, uNormalRows[2].x, distance(aPosition, eye));
        vec3 position = aPosition + aTangent.xyz * morph;
        gl_Position = uMvp * vec4(position, 1.0);
        vViewDepth = gl_Position.w;
        vNormal = aNormal;
        vTangent = vec4(aTangent.xyz, 1.0);
        vColour = aColour;
        vUv = aUv;
        vTerrain = 0.0;
        return;
    }
    gl_Position = uMvp * vec4(aPosition, 1.0);
    vViewDepth = gl_Position.w;
    vNormal = vec3(dot(uNormalRows[0].xyz, aNormal),
                   dot(uNormalRows[1].xyz, aNormal),
                   dot(uNormalRows[2].xyz, aNormal));
    vTangent = vec4(dot(uNormalRows[0].xyz, aTangent.xyz),
                    dot(uNormalRows[1].xyz, aTangent.xyz),
                    dot(uNormalRows[2].xyz, aTangent.xyz), aTangent.w);
    vUv = aUv;
    vColour = vec4(aColour.rgb * vec3(uNormalRows[0].w, uNormalRows[1].w, uNormalRows[2].w), aColour.a);
    vTerrain = -1.0;
}
