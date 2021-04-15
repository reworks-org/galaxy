///
/// Phong.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADER_PHONG_HPP_
#define GALAXY_GRAPHICS_SHADER_PHONG_HPP_

#include <string>

namespace galaxy
{
	namespace shaders
	{
		///
		/// Phong fragment shader.
		///
		inline const std::string phong_frag = R"(
			#version 450 core

			struct Light
			{
				vec3 ambient_intensity;
				vec3 diffuse_intensity;
				vec3 specular_intensity;
			};

			struct Material
			{
				vec3 ambient;
				vec3 diffuse;
				vec3 specular;
				float shininess;
			};

			in vec2 io_texels;
			in vec3 io_normals;
			in vec3 io_frag_pos;
			out vec4 io_frag_colour;

			uniform vec3 u_light_pos;
			uniform vec3 u_camera_pos;

			uniform sampler2D tex_DIFFUSE1;
			uniform sampler2D tex_SPECULAR1;
			uniform sampler2D tex_NORMAL1;
			
			uniform Light light;
			uniform Material material;
			
			void main()
			{
				vec4 diffmap = texture(tex_DIFFUSE1, io_texels);

				vec3 ambient = light.ambient_intensity * (material.ambient * diff_map.rgb);
				
				vec3 norm = normalize(io_normals);
				vec3 light_dir = normalize(u_light_pos - io_frag_pos);

				float diff = max(dot(norm, light_dir), 0.0);
				vec3 diffuse = light.diffuse_intensity * diff * (material.diffuse * diff_map.rgb);
				
				vec3 view_dir = normalize(u_camera_pos - io_frag_pos);
				vec3 reflect_dir = reflect(-light_dir, norm);

				float spec_calc = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
				vec3 specular = light.specular_intensity * spec_calc * (material.specular * vec3(texture(tex_SPECULAR1, io_texels)));

				vec3 result = ambient + diffuse + specular;
				io_frag_colour = vec4(result, diff_map.a);
			}
		)";
	} // namespace shaders
} // namespace galaxy

#endif