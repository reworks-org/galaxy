///
/// Cubemap.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADER_CUBEMAP_HPP_
#define GALAXY_GRAPHICS_SHADER_CUBEMAP_HPP_

#include <string>

namespace galaxy
{
	namespace shaders
	{
		///
		/// Cubemap vertex shader.
		///
		inline const std::string cubemap_vert = R"(
			#version 450 core
			layout(location = 0) in vec3 l_pos;

			out vec3 io_texels;

			uniform mat4 u_view_matrix;
			uniform mat4 u_camera_proj;

			void main()
			{
				io_texels = l_pos;

				vec4 pos = u_camera_proj * u_view_matrix * vec4(l_pos, 1.0);
				gl_Position = pos.xyww;
			}
		)";

		///
		/// Cubemap fragment shader.
		///
		inline const std::string cubemap_frag = R"(
			#version 450 core

			in vec3 io_texels;
			out vec4 io_frag_colour;

			uniform samplerCube cubemap;

			void main()
			{
				io_frag_colour = texture(cubemap, io_texels);
			}
		)";
	} // namespace shaders
} // namespace galaxy

#endif