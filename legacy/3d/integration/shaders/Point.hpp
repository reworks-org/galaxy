///
/// Point.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADERS_POINT_HPP_
#define GALAXY_GRAPHICS_SHADERS_POINT_HPP_

namespace galaxy
{
	namespace shaders
	{
		///
		/// Point vertex shader.
		///
		inline constexpr const char* const point_vert = R"(
			#version 450 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec4 l_colour;

			out vec4 io_colour;

			uniform mat4 u_cameraProj;
			uniform mat4 u_cameraView;
			uniform mat4 u_transform;
			uniform int u_point_size;

			void main()
			{
				gl_PointSize = u_point_size;
				gl_Position  = u_cameraProj * u_cameraView * u_transform * vec4(l_pos, 0.0, 1.0);
				io_colour    = l_colour;
			}
		)";

		///
		/// Point fragment shader.
		///
		inline constexpr const char* const point_frag = R"(
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