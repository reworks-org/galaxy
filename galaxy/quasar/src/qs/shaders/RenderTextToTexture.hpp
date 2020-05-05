///
/// RenderTextToTexture.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERTEXTTOTEXTURE_HPP_
#define QUASAR_RENDERTEXTTOTEXTURE_HPP_

#include <string>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Vertex shader for drawing RenderTextToTextures.
	///
	static inline std::string s_renderTextToTextureVS = R""(
		#version 450 core
		layout(location = 0) in vec2 l_pos;
		layout(location = 1) in vec4 l_colour;
		layout(location = 2) in vec2 l_texels;

		out vec4 io_colour;
		out vec2 io_texels;

		uniform mat4 u_projection;

		void main()
		{
			gl_Position = u_projection * vec4(l_pos, 0.0, 1.0);
			io_colour = l_colour;
			io_texels = l_texels;
		}
)"";

	///
	/// Fragment shader for drawing RenderTextToTextures.
	///
	static inline std::string s_renderTextToTextureFS = R""(
		#version 450 core

		in vec4 io_colour;
		in vec2 io_texels;

		out vec4 io_frag_colour;

		uniform sampler2D text;

		void main()
		{
			vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, io_texels).r);
			io_frag_colour = io_colour * sampled;
		}
)"";
}

#endif