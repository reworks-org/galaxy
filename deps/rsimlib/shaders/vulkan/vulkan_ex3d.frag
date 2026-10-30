#version 450
layout(location = 0) in vec3 vColour;
layout(location = 0) out vec4 fragColor;
void main() 
{
    fragColor = vec4(vColour, 1.0);
}