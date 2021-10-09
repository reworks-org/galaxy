///
/// Skybox.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADERS_SKYBOX_HPP_
#define GALAXY_GRAPHICS_SHADERS_SKYBOX_HPP_

namespace galaxy
{
	namespace shaders
	{
		///
		/// Skybox vertex shader.
		///
		inline constexpr const char* const skybox_vert = R"(
			#version 450 core
			layout(location = 0) in vec3 l_pos;

			layout (std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_view;
				mat4 u_camera_proj;
				vec3 u_camera_pos;
			};

			out vec3 io_texels;

			void main()
			{
				mat4 view = mat4(mat3(u_camera_view));
				vec4 pos = u_camera_proj * view * vec4(l_pos, 1.0);
	
				gl_Position = pos.xyww;
				io_texels = vec3(l_pos.xy, -l_pos.z);
			}
		)";

		///
		/// Skybox fragment shader.
		///
		inline constexpr const char* const skybox_frag = R"(
			#version 450 core

			in vec3 io_texels;
			out vec4 io_frag_colour;

			uniform samplerCube skybox;

			void main()
			{
				io_frag_colour = texture(skybox, io_texels);
			}
		)";
	} // namespace shaders
} // namespace galaxy

#endif