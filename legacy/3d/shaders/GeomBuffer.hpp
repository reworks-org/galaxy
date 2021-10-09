///
/// GeomBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADERS_GEOMBUFFER_HPP_
#define GALAXY_GRAPHICS_SHADERS_GEOMBUFFER_HPP_

namespace galaxy
{
	namespace shaders
	{
		///
		/// GeomBuffer vertex shader.
		///
		inline constexpr const char* const geom_vert = R"(
            #version 450 core
            layout(location = 0) in vec3 l_pos;
            layout(location = 1) in vec3 l_normals;
            layout(location = 2) in vec3 l_tangents;
            layout(location = 3) in vec2 l_texels;

            layout (std140, binding = 0) uniform camera_data
            {
	            mat4 u_camera_view;
	            mat4 u_camera_proj;
	            vec3 u_camera_pos;
            };

            out VS_OUT
            {
                vec2 texels;
                vec3 frag_pos;
                vec3 normals;
                mat3 tbn;
            } vs_out;

            void main()
            {
	            vs_out.frag_pos = l_pos;
	            vs_out.texels = l_texels;
                vs_out.normals = l_normals;
	
	            vec3 T = normalize(l_tangents);
	            vec3 N = normalize(l_normals);

	            T = normalize(T - dot(T, N) * N);
	            vec3 B = cross(N, T);
		
	            vs_out.tbn = mat3(T, B, N);
	            gl_Position = u_camera_proj * u_camera_view * vec4(l_pos, 1.0);
            }
		)";

		///
		/// GeomBuffer fragment shader.
		///
		inline constexpr const char* const geom_frag = R"(
            #version 450 core

            layout(location = 0) out vec3 l_pos;
            layout(location = 1) out vec3 l_diffuse;
            layout(location = 2) out vec4 l_specular;
            layout(location = 3) out vec3 l_normals;

            struct Material
            {
                sampler2D diffuse;
                sampler2D specular;
                sampler2D normal;
                vec4 diffuse_colours;
                vec3 specular_colours;
                bool use_diffuse_texture;
                bool use_specular_texture;
                bool use_normal_texture;
                float shininess;
            };

            in VS_OUT
            {
                vec2 texels;
                vec3 frag_pos;
                vec3 normals;
                mat3 tbn;
            } fs_in;

            uniform Material material;

            void main()
            {
                l_pos = fs_in.frag_pos;
    
                if (material.use_diffuse_texture)
                {
                    l_diffuse = texture(material.diffuse, fs_in.texels).rgb;
                }
                else
                {
                    l_diffuse = material.diffuse_colours.rgb;
                }

                if (material.use_specular_texture)
                {
                    l_specular = texture(material.specular, fs_in.texels);
                }
                else
                {
                    l_specular = vec4(material.specular_colours, 1.0);
                }
                
                // Store shininess as alpha of specular map.
                l_specular.a = material.shininess;

                vec3 normals = vec3(0);
                if (material.use_normal_texture)
                {
                    normals = texture(material.normal, fs_in.texels).rgb;
                    normals = normals * 2.0 - 1.0;
                    normals = normalize(fs_in.tbn * normals);
                }
                else
                {
                    normals = normalize(fs_in.tbn * fs_in.normals);
                }

                l_normals = normals;
            }
		)";
	} // namespace shaders
} // namespace galaxy

#endif