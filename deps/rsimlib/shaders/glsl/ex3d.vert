#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColour;

uniform mat4 uMvp;

out vec3 vColour;

void main()
{
    gl_Position = uMvp * vec4(aPosition, 1.0);
    vColour = aColour;
}