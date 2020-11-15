///
/// Glyph.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_GLYPH_SHADER_HPP_
#define QUASAR_GLYPH_SHADER_HPP_

#include <string>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Shader identification.
	///
	namespace shaders
	{
		///
		/// Glyph vertex shader.
		///
		inline const std::string glyph_vs = R"(
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
		inline const std::string glyph_fs = R"(
			#version 450 core

			in vec2 io_texels;
			out vec4 io_frag_colour;

			uniform sampler2D u_text;

			void main()
			{    
				vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_text, io_texels).r);
				io_frag_colour = vec4(1.0, 1.0f, 1.0, 1.0) * sampled;
			} 
		)";
	} // namespace shaders
} // namespace qs

#endif