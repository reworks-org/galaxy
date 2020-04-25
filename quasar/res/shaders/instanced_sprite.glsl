#shader vertex
#version 450 core
layout(location = 0) in vec2 l_pos;
layout(location = 1) in vec4 l_colour;
layout(location = 2) in vec2 l_texels;

out vec4 io_colour;
out vec2 io_texels;

uniform mat4 u_cameraView;
uniform mat4 u_cameraProj;
uniform float u_width;
uniform float u_height;

void main()
{
	mat4 camera = u_cameraProj * u_cameraView;
	gl_Position =  camera * vec4(l_pos, 0.0, 1.0);
	
	io_colour = l_colour;
	io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));
}

#shader fragment
#version 450 core

in vec2 io_texels;
in vec4 io_colour;

out vec4 io_frag_colour;

uniform sampler2D u_texture;

void main()
{
	vec4 tex = texture(u_texture, io_texels);
    tex.a *= io_colour.a;
	io_frag_colour = tex;
}


uniform vec2 offsets[100];
  vec2 offset = offsets[gl_InstanceID];
    gl_Position = vec4(aPos + offset, 0.0, 1.0);