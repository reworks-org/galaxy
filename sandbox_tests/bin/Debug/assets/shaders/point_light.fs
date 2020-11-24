#version 450 core

in vec2 io_texels;
in vec2 io_resolution;

out vec4 io_frag_colour;

uniform vec3 u_falloff;
uniform vec3 u_light_pos;
uniform vec4 u_light_colour;
uniform vec4 u_ambient_colour;
uniform sampler2D u_texture;

void main()
{
	//RGBA of our diffuse color
	vec4 diffuse_colour = texture(u_texture, io_texels);
	
	//RGB of our normal map
	vec3 normal_map = texture(u_texture, io_texels).rgb;
	
	//The delta position of light
	vec3 light_dir = vec3(u_light_pos.xy - (gl_FragCoord.xy / io_resolution.xy), u_light_pos.z);
	
	//Correct for aspect ratio
	light_dir.x *= io_resolution.x / io_resolution.y;
	
	//Determine distance (used for attenuation) BEFORE we normalize our light_dir
	float D = length(light_dir);
	
	//normalize our vectors
	vec3 N = normalize(normal_map * 2.0 - 1.0);
	vec3 L = normalize(light_dir);
	
	//Pre-multiply light color with intensity
	//Then perform "N dot L" to determine our diffuse term
	vec3 diffuse = (u_light_colour.rgb * u_light_colour.a) * max(dot(N, L), 0.0);

	//pre-multiply ambient color with intensity
	vec3 ambient = u_ambient_colour.rgb * u_ambient_colour.a;
	
	//calculate attenuation
	float attenuation = 1.0 / ( u_falloff.x + (u_falloff.y*D) + (u_falloff.z*D*D) );
	
	//the calculation which brings it all together
	vec3 intensity = ambient + diffuse * attenuation;
	vec3 final = diffuse_colour.rgb * intensity;
	io_frag_colour = vec4(final, diffuse_colour.a);
}