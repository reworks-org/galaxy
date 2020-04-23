#version 450 core

in vec2 io_texels;
in vec4 io_colour;

out vec4 io_frag_colour;

uniform vec2 u_window_resolution;
uniform vec3 u_falloff;
uniform vec3 u_light_pos;
uniform vec4 u_light_colour;
uniform vec4 u_ambient_colour;
uniform sampler2D u_texture;

void main()
{
    // Diffuse i.e. the pixel from the texture.
	vec4 tex = texture(u_texture, io_texels);

    // Need to generate a normal map somehow. Heightmaps? Random? dunno.
    vec3 normals = vec3(0.0, 0.0, 1.0);

    // Light delta position.
    //vec3 direction = vec3(io_adj_light_pos.xy - (gl_FragCoord.xy / u_window_resolution.xy), io_adj_light_pos.z);
    vec3 direction = vec3(u_light_pos - vec3(gl_FragCoord.xy, 0.0)) / vec3(u_window_resolution.xy, 1.0);

    // Aspect ratio correction.
    direction.x *= u_window_resolution.x / u_window_resolution.y;

    // Distance - for attenuation.
    float D = length(direction);

    // Normalize vectors.
    vec3 N = normalize(normals * 2.0 - 1.0);
    vec3 L = normalize(direction);

    // Calc intensity and then perform "N dot L" to get diffuse term.
    vec3 diffuse = (u_light_colour.rgb * u_light_colour.a) * max(dot(N, L), 1.0);

    // Calc ambient light.
    vec3 ambient = u_ambient_colour.rgb * u_ambient_colour.a;

    // Calc attenuation.
    float attenuation = 1.0 / ( u_falloff.x + (u_falloff.y*D) + (u_falloff.z*D*D) );

    // Calc overall light.
    vec3 intensity = ambient + diffuse * attenuation;
    vec3 final = tex.rgb * intensity;

    // add in sprite tinting here?
	io_frag_colour = /*io_colour * */vec4(final, tex.a);
}