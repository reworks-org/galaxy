///
/// Skybox.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADER_SKYBOX_HPP_
#define GALAXY_GRAPHICS_SHADER_SKYBOX_HPP_

#include <string>

namespace galaxy
{
	namespace shaders
	{
		///
		/// Skybox vertex shader.
		///
		inline const std::string skybox_vert = R"(
			#version 450 core
			layout(location = 0) in vec3 l_pos;

			out vec3 io_texels;

			uniform mat4 u_view_matrix;
			uniform mat4 u_camera_proj;

			void main()
			{
				io_texels = vec3(l_pos.xy, -l_pos.z);
				
				vec4 pos = u_camera_proj * u_view_matrix * vec4(l_pos, 1.0);
				gl_Position = pos.xyww;
			}
		)";

		///
		/// Skybox fragment shader.
		///
		inline const std::string skybox_frag = R"(
			#version 450 core

			in vec3 io_texels;
			out vec4 io_frag_colour;

			uniform samplerCube skybox;

			void main()
			{
				io_frag_colour = texture(skybox, io_texels);
			}
		)";
	} // namespace shaders
} // namespace galaxy

#endif