///
/// Line.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_GRAPHICS_SHADERS_LINE_HPP_
#define QUASAR_GRAPHICS_SHADERS_LINE_HPP_

#include <string>

namespace galaxy
{
	namespace shaders
	{
		///
		/// Line vertex shader.
		///
		inline const std::string line_vert = R"(
			#version 450 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec4 l_colour;

			out vec4 io_colour;

			uniform mat4 u_cameraProj;
			uniform mat4 u_cameraView;
			uniform mat4 u_transform;

			void main()
			{
				gl_Position = u_cameraProj * u_cameraView * u_transform * vec4(l_pos, 0.0, 1.0);
				io_colour   = l_colour;
			}
		)";

		///
		/// Line fragment shader.
		///
		inline const std::string line_frag = R"(
			#version 450 core

			in vec4 io_colour;

			out vec4 io_frag_colour;

			void main()
			{
				io_frag_colour = io_colour;
			}
		)";
	} // namespace shaders
} // namespace galaxy

#endif