#version 430 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uProjection;

out vec2 uv;

void main()
{
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
    uv = aTexCoord;
}