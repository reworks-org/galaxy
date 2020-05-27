///
/// Glyphs.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_GLYPHS_HPP_
#define QUASAR_GLYPHS_HPP_

#include <string>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Vertex shader for drawing glyphs.
	///
	static inline std::string s_glyphsVS = R""(
		#version 450 core
		layout(location = 0) in vec2 l_pos;
		layout(location = 1) in vec2 l_texels;
		layout(location = 2) in float l_opacity;

		out float io_opacity;
		out vec2 io_texels;

		uniform mat4 u_projection;

		void main()
		{
			gl_Position = u_projection * vec4(l_pos, 0.0, 1.0);
	
			io_opacity = l_opacity;
			io_texels = l_texels;
		}
)"";

	///
	/// Fragment shader for drawing glyphs.
	///
	static inline std::string s_glyphsFS = R""(
		#version 450 core

		in float io_opacity;
		in vec2 io_texels;

		out vec4 io_frag_colour;

		uniform sampler2D glyph;

		void main()
		{
			vec4 sampled = vec4(1.0, 1.0, 1.0, texture(glyph, io_texels).r);
			io_frag_colour.a = sampled.a * io_opacity;
		}
)"";
}

#endif