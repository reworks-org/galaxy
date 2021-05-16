///
/// VAO.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADERS_VAO_HPP_
#define GALAXY_GRAPHICS_SHADERS_VAO_HPP_

namespace galaxy
{
	namespace shaders
	{
		///
		/// Plain VAO vertex shader.
		///
		inline constexpr const char* const vao_vert = R"(
			#version 450 core
			layout (location = 0) in vec3 l_pos;
			layout (location = 1) in vec2 l_texels;
	
			out vec2 io_texels;
	
			void main()
			{
				io_texels = l_texels;
				gl_Position = vec4(l_pos, 1.0);
			}
		)";

		///
		/// Plain VAO fragment shader.
		///
		inline constexpr const char* const vao_frag = R"(
			#version 450 core

			in vec2 io_texels;
			out vec4 io_frag_colour;
	
			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour = texture(u_texture, io_texels);
			}
		)";
	} // namespace shaders
} // namespace galaxy

#endif