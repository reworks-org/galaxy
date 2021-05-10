///
/// RenderToTexture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADERS_RENDERTOTEXTURE_HPP_
#define GALAXY_GRAPHICS_SHADERS_RENDERTOTEXTURE_HPP_

namespace galaxy
{
	namespace shaders
	{
		///
		/// RenderToTexture vertex shader.
		///
		inline constexpr const char* const render_to_texture_vert = R"(
			#version 450 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;

			out vec2 io_texels;

			uniform mat4 u_projection;
			uniform mat4 u_transform;
			uniform float u_width;
			uniform float u_height;

			void main()
			{
				gl_Position = u_projection * u_transform * vec4(l_pos, 0.0, 1.0);
				io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));
			}
		)";

		///
		/// RenderToTexture fragment shader.
		///
		inline constexpr const char* const render_to_texture_frag = R"(
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