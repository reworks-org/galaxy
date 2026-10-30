#version 450
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(push_constant) uniform Projection { mat4 value; } projection;
layout(location = 0) out vec2 uv;
void main() { gl_Position = projection.value * vec4(aPos, 0.0, 1.0); uv = aTexCoord; }