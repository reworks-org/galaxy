///
/// Glyph.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADERS_GLYPH_HPP_
#define GALAXY_GRAPHICS_SHADERS_GLYPH_HPP_

namespace galaxy
{
	namespace shaders
	{
		///
		/// Glyph vertex shader.
		///
		inline constexpr const char* const glyph_vert = R"(
			#version 450 core
			layout (location = 0) in vec4 vertex;
		
			out vec2 io_texels;

			uniform mat4 u_proj;

			void main()
			{
				gl_Position = u_proj * vec4(vertex.xy, 0.0, 1.0);
				io_texels = vertex.zw;
			}
		)";

		///
		/// Glyph fragment shader.
		///
		inline constexpr const char* const glyph_frag = R"(
			#version 450 core

			in vec2 io_texels;
			out vec4 io_frag_colour;

			uniform sampler2D u_text;

			void main()
			{    
				io_frag_colour = vec4(1.0, 1.0, 1.0, texture(u_text, io_texels).r);
			} 
		)";
	} // namespace shaders
} // namespace galaxy

#endif