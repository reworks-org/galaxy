///
/// LightObject.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADER_LIGHTOBJECT_HPP_
#define GALAXY_GRAPHICS_SHADER_LIGHTOBJECT_HPP_

#include <string>

namespace galaxy
{
	namespace shaders
	{
		///
		/// LightObject vertex shader.
		///
		inline const std::string light_object_vert = R"(
			#version 450 core
			layout(location = 0) in vec3 l_pos;

			uniform mat4 u_transform;
			uniform mat4 u_cameraView;
			uniform mat4 u_cameraProj;

			void main()
			{
				gl_Position = u_cameraProj * u_cameraView * u_transform * vec4(l_pos, 1.0);
			}
		)";

		///
		/// LightObject fragment shader.
		///
		inline const std::string light_object_frag = R"(
			#version 450 core

			out vec4 io_frag_colour;

			void main()
			{
				io_frag_colour = vec4(1.0);
			}
		)";
	} // namespace shaders
} // namespace galaxy

#endif