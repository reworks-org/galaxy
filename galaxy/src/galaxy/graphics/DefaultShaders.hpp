///
/// DefaultShaders.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_DEFAULTSHADERS_HPP_
#define GALAXY_GRAPHICS_DEFAULTSHADERS_HPP_

namespace galaxy
{
	namespace shaders
	{
		///
		/// Sprite vertex shader.
		///
		constexpr const auto sprite_vert = R"(
			#version 460 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;

			layout(std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_model_view;
				mat4 u_camera_proj;
			};

			out vec2 io_texels;
	
			uniform mat4 u_transform;
	
			void main()
			{
				gl_Position =  u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);

				io_texels = l_texels;
				io_texels.y = 1.0 - io_texels.y;
			}
		)";

		///
		/// Sprite fragment shader.
		///
		constexpr const auto sprite_frag = R"(
			#version 460 core

			in vec2 io_texels;
			out vec4 io_frag_colour;

			uniform float u_opacity;
			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour = texture(u_texture, io_texels);
				io_frag_colour.a *= u_opacity;
			}
		)";

		///
		/// Map vertex shader.
		///
		constexpr const auto map_vert = R"(
			#version 460 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;

			layout(std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_model_view;
				mat4 u_camera_proj;
			};

			out vec2 io_texels;
	
			uniform mat4 u_transform;
	
			void main()
			{
				gl_Position =  u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);

				io_texels = l_texels;
				io_texels.y = 1.0 - io_texels.y;
			}
		)";

		///
		/// Map fragment shader.
		///
		constexpr const auto map_frag = R"(
			#version 460 core

			in vec2 io_texels;
			out vec4 io_frag_colour;

			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour = texture(u_texture, io_texels);
			}
		)";

		///
		/// Point vertex shader.
		///
		constexpr const auto point_vert = R"(
			#version 460 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;

			layout(std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_model_view;
				mat4 u_camera_proj;
			};
	
			uniform mat4 u_transform;

			void main()
			{
				gl_PointSize = 4;
				gl_Position  = u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);
			}
		)";

		///
		/// Point fragment shader.
		///
		constexpr const auto point_frag = R"(
			#version 460 core

			out vec4 io_frag_colour;
	
			uniform vec4 u_colour;

			void main()
			{
				io_frag_colour = u_colour;
			}
		)";

		///
		/// Line vertex shader.
		///
		constexpr const auto line_vert = R"(
			#version 460 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;

			layout(std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_model_view;
				mat4 u_camera_proj;
			};

			uniform mat4 u_transform;

			void main()
			{
				gl_Position = u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);
			}
		)";

		///
		/// Line fragment shader.
		///
		constexpr const auto line_frag = R"(
			#version 460 core

			out vec4 io_frag_colour;

			uniform vec4 u_colour;

			void main()
			{
				io_frag_colour = u_colour;
			}
		)";

		///
		/// Render To Texture vertex shader.
		///
		constexpr const auto render_to_texture_vert = R"(
			#version 460 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;

			out vec2 io_texels;
	
			uniform mat4 u_projection;
			uniform mat4 u_transform;

			void main()
			{
				gl_Position = u_projection * u_transform * vec4(l_pos, 0.0, 1.0);

				io_texels = l_texels;
				io_texels.y = 1.0 - io_texels.y;
			}
		)";

		///
		/// Render To Texture fragment shader.
		///
		constexpr const auto render_to_texture_frag = R"(
			#version 460 core

			in vec2 io_texels;
			out vec4 io_frag_colour;

			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour = texture(u_texture, io_texels);
			}
		)";

		///
		/// Text vertex shader.
		///
		constexpr const auto text_vert = R"(
			#version 460 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;

			layout(std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_model_view;
				mat4 u_camera_proj;
			};

			out vec2 io_texels;
	
			uniform mat4 u_transform;
	
			void main()
			{
				gl_Position =  u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);

				io_texels = l_texels;
				io_texels.y = 1.0 - io_texels.y;
			}
		)";

		///
		/// Text fragment shader.
		///
		constexpr const auto text_frag = R"(
			#version 460 core

			in vec2 io_texels;
			out vec4 io_frag_colour;

			uniform vec4 u_colour;
			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour = texture(u_texture, io_texels) * u_colour;
			}
		)";
	} // namespace shaders
} // namespace galaxy

#endif