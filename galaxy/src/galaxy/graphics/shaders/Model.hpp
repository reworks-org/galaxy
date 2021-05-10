///
/// Model.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADERS_MODEL_HPP_
#define GALAXY_GRAPHICS_SHADERS_MODEL_HPP_

namespace galaxy
{
	namespace shaders
	{
		///
		/// Model vertex shader.
		///
		inline constexpr const char* const model_vert = R"(
			#version 450 core
			
			layout(location = 0) in vec3 l_pos;
			layout(location = 1) in vec3 l_normals;
			layout(location = 2) in vec3 l_tangents;
			layout(location = 3) in vec2 l_texels;
			
			layout (std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_view;
				mat4 u_camera_proj;
				vec3 u_camera_pos;
			};
			
			out vec2 io_texels;
			out vec3 io_frag_pos;
			out vec3 io_normals;
			out mat3 io_tbn;
			
			void main()
			{
				io_texels = l_texels;
				io_normals = l_normals;
				io_frag_pos = l_pos;
				
				vec3 T = normalize(l_tangents);
				vec3 N = normalize(l_normals);

				T = normalize(T - dot(T, N) * N);
				vec3 B = cross(N, T);
								
				io_tbn = mat3(T, B, N);
				gl_Position = u_camera_proj * u_camera_view * vec4(l_pos, 1.0);
			}
		)";
	} // namespace shaders
} // namespace galaxy

#endif