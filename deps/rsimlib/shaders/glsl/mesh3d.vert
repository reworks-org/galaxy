#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColour;
layout(location = 3) in vec2 aUv;

uniform mat4 uMvp;
// Rows of the normal matrix; w holds the tint colour.
uniform vec4 uNormalRows[3];

out vec3 vNormal;
out vec3 vColour;
out vec2 vUv;

void main()
{
    gl_Position = uMvp * vec4(aPosition, 1.0);
    vNormal = vec3(dot(uNormalRows[0].xyz, aNormal),
                   dot(uNormalRows[1].xyz, aNormal),
                   dot(uNormalRows[2].xyz, aNormal));
    vUv = aUv;
    vColour = aColour.rgb * vec3(uNormalRows[0].w, uNormalRows[1].w, uNormalRows[2].w);
}
