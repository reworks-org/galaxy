///
/// Lines.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_LINES_HPP_
#define QUASAR_LINES_HPP_

#include <string>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Vertex shader for drawing lines.
	///
	static inline std::string s_linesVS = R""(
		#version 450 core
		layout(location = 0) in vec2 l_pos;

		out vec4 io_colour;

		uniform mat4 u_cameraProj;
		uniform mat4 u_cameraView;
		uniform vec4 u_colour;

		void main()
		{
			gl_Position = u_cameraProj * u_cameraView * vec4(l_pos, 0.0, 1.0);
			io_colour = u_colour;
		}
)"";

	///
	/// Fragment shader for drawing lines.
	///
	static inline std::string s_linesFS = R""(
		#version 450 core

		in vec4 io_colour;

		out vec4 io_frag_colour;

		void main()
		{
			io_frag_colour = io_colour;
		}
)"";
}

#endif