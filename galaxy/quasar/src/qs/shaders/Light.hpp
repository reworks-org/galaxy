///
/// Light.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_LIGHT_HPP_
#define QUASAR_LIGHT_HPP_

#include <string>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Vertex shader for drawing Lights.
	///
    static inline std::string s_lightVS = R""(
        #version 450 core
		layout(location = 0) in vec2 l_pos;
		layout(location = 1) in vec4 l_colour;
		layout(location = 2) in vec2 l_texels;

		out vec2 io_texels;
		out vec4 io_colour;

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
)"";

	///
	/// Fragment shader for drawing Lights.
	///
    static inline std::string s_lightFS = R""(
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
	        io_frag_colour = io_colour.a * vec4(final, tex.a);
        }
)"";
}

#endif