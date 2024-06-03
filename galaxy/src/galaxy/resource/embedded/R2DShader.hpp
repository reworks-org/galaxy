///
/// R2DShader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_EMBEDDED_R2DSHADER_HPP_
#define GALAXY_RESOURCE_EMBEDDED_R2DSHADER_HPP_

namespace galaxy
{
	namespace resource
	{
		///
		/// Vertex shader for the 2d renderer.
		///
		constexpr const auto r2d_vert_shader = R"(
			#version 460 core

			precision highp int;
			precision highp float;

			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;

			layout(std430, binding = 0) readonly buffer camera_data
			{
				mat4 u_camera_model_view;
				mat4 u_camera_proj;
			};

			layout(std430, binding = 1) readonly buffer render_data
			{
				mat4 u_transform;
				vec4 u_colour;
				int u_entity;
				bool u_point;
				bool u_textured;
			};

			out vec2 io_texels;
	
			void main()
			{
				gl_Position =  u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);

				io_texels = l_texels;
				io_texels.y = 1.0 - io_texels.y;

				if (u_point)
				{
					gl_PointSize = 4;
				}
			}
		)";

		///
		/// Fragment shader for the 2d renderer.
		///
		constexpr const auto r2d_frag_shader = R"(
			#version 460 core

			precision highp int;
			precision highp float;

			layout(std430, binding = 0) readonly buffer camera_data
			{
				mat4 u_camera_model_view;
				mat4 u_camera_proj;
			};

			layout(std430, binding = 1) readonly buffer render_data
			{
				mat4 u_transform;
				vec4 u_colour;
				int u_entity;
				bool u_point;
				bool u_textured;
			};

			in vec2 io_texels;

			layout (location = 0) out vec4 io_frag_colour;
			layout (location = 1) out int io_entity;

			uniform sampler2D u_texture;

			void main()
			{
				if (u_textured)
				{
					io_frag_colour = texture(u_texture, io_texels) * u_colour;
				}
				else
				{
					io_frag_colour = u_colour;
				}

				io_entity = u_entity;
			}
		)";
	} // namespace resource
} // namespace galaxy

#endif
