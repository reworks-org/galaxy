#version 430 core

// A fullscreen triangle at the far plane. vNdc interpolates to [-1, 1] over the screen.
out vec2 vNdc;

void main()
{
    vec2 corner = vec2(float((gl_VertexID << 1) & 2), float(gl_VertexID & 2));
    vNdc = corner * 2.0 - 1.0;
    gl_Position = vec4(vNdc, 1.0, 1.0);
}
