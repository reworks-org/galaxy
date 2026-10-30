#version 450
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColour;
layout(push_constant) uniform Transform { mat4 mvp; } transform;
layout(location = 0) out vec3 vColour;
void main() 
{ 
    gl_Position = transform.mvp * vec4(aPosition, 1.0); 
    vColour = aColour; 
}