#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
  
out vec4 vertexColor;

void main()
{
    gl_Position = vec4(pos, 1.0);
    vertexColor = vec4(col, 1.0);
}