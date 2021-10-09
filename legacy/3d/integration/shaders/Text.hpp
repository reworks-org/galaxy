///
/// Text.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADERS_TEXT_HPP_
#define GALAXY_GRAPHICS_SHADERS_TEXT_HPP_

namespace galaxy
{
	namespace shaders
	{
		///
		/// Text vertex shader.
		///
		inline constexpr const char* const text_vert = R"(
			#version 450 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;
			layout(location = 2) in float l_opacity;

			out vec2 io_texels;
			out float io_opacity;

			uniform mat4 u_cameraView;
			uniform mat4 u_cameraProj;
			uniform float u_width;
			uniform float u_height;

			void main()
			{
				mat4 camera = u_cameraProj * u_cameraView;
				gl_Position =  camera * vec4(l_pos, 0.0, 1.0);

				io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));
				io_opacity = l_opacity;
			}
		)";

		///
		/// Text fragment shader.
		///
		inline constexpr const char* const text_frag = R"(
			#version 450 core

			in vec2 io_texels;
			in float io_opacity;
			out vec4 io_frag_colour;

			uniform vec4 u_colour;
			uniform sampler2D u_texture;

			void main()
			{
				vec4 corrected = vec4(u_colour.xyz, io_opacity);
				io_frag_colour = texture(u_texture, io_texels) * corrected;
			}
		)";
	} // namespace shaders
} // namespace galaxy

#endif