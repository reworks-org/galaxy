///
/// Texts.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXTS_HPP_
#define QUASAR_TEXTS_HPP_

#include <string>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Vertex shader for drawing text.
	///
	static inline std::string s_textVS = R""(
		#version 450 core
		layout(location = 0) in vec2 l_pos;
		layout(location = 1) in vec2 l_texels;
		layout(location = 2) in float l_opacity;

		out vec2 io_texels;
		out vec4 io_colour;

		uniform mat4 u_cameraView;
		uniform mat4 u_cameraProj;
		uniform mat4 u_transform;
		uniform float u_opacity;

		void main()
		{
			mat4 camera = u_cameraProj * u_cameraView;
			gl_Position =  camera * u_transform * vec4(l_pos, 0.0, 1.0);
	
			io_colour = vec4(0.0, 0.0, 0.0, u_opacity);
			io_texels = l_texels;
		}
)"";

	///
	/// Fragment shader for drawing text.
	///
	static inline std::string s_textFS = R""(
		#version 450 core

		in vec2 io_texels;
		in vec4 io_colour;

		out vec4 io_frag_colour;

		uniform sampler2D u_texture;

		void main()
		{
			vec4 tex = texture(u_texture, io_texels);
			tex.a *= io_colour.a;

			io_frag_colour = tex;
		}
)"";
}

#endif