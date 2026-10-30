#version 450

// A fullscreen triangle at the far plane. vNdc interpolates to [-1, 1] over the screen.
layout(location = 0) out vec2 vNdc;

void main()
{
    vec2 corner = vec2(float((gl_VertexIndex << 1) & 2), float(gl_VertexIndex & 2));
    vNdc = corner * 2.0 - 1.0;
    gl_Position = vec4(vNdc, 1.0, 1.0);
}
