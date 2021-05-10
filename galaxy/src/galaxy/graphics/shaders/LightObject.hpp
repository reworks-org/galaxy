///
/// LightObject.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADERS_LIGHTOBJECT_HPP_
#define GALAXY_GRAPHICS_SHADERS_LIGHTOBJECT_HPP_

namespace galaxy
{
	namespace shaders
	{
		///
		/// LightObject vertex shader.
		///
		inline constexpr const char* const light_object_vert = R"(
			#version 450 core
			layout(location = 0) in vec3 l_pos;

			layout (std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_view;
				mat4 u_camera_proj;
				vec3 u_camera_pos;
			};

			uniform mat4 u_transform;

			void main()
			{
				gl_Position = u_camera_proj * u_camera_view * u_transform * vec4(l_pos, 1.0);
			}
		)";

		///
		/// LightObject fragment shader.
		///
		inline constexpr const char* const light_object_frag = R"(
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