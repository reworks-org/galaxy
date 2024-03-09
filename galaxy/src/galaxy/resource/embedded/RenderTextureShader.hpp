///
/// RenderTextureShader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_EMBEDDED_RENDERTEXTURESHADER_HPP_
#define GALAXY_RESOURCE_EMBEDDED_RENDERTEXTURESHADER_HPP_

namespace galaxy
{
	namespace resource
	{
		///
		/// Render To Texture vertex shader.
		///
		constexpr const auto render_texture_shader_vert = R"(
			#version 460 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;

			out vec2 io_texels;
	
			uniform mat4 u_projection;
			uniform mat4 u_transform;

			void main()
			{
				gl_Position = u_projection * u_transform * vec4(l_pos, 0.0, 1.0);

				io_texels = l_texels;
				io_texels.y = 1.0 - io_texels.y;
			}
		)";

		///
		/// Render To Texture fragment shader.
		///
		constexpr const auto render_texture_shader_frag = R"(
			#version 460 core

			in vec2 io_texels;
			out vec4 io_frag_colour;

			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour = texture(u_texture, io_texels);
			}
		)";
	} // namespace resource
} // namespace galaxy

#endif
