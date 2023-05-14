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

			layout (location = 0) out vec4 io_frag_colour;
			layout (location = 1) out int io_entity;

			uniform int u_entity;
			uniform float u_opacity;
			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour = texture(u_texture, io_texels);
				io_frag_colour.a *= u_opacity;

				io_entity = u_entity;
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

			layout (location = 0) out vec4 io_frag_colour;
			layout (location = 1) out int io_entity;

			uniform int u_entity;
			uniform vec4 u_colour;

			void main()
			{
				io_frag_colour = u_colour;
				io_entity = u_entity;
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

			layout (location = 0) out vec4 io_frag_colour;
			layout (location = 1) out int io_entity;

			uniform int u_entity;
			uniform vec4 u_colour;

			void main()
			{
				io_frag_colour = u_colour;
				io_entity = u_entity;
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

			layout (location = 0) out vec4 io_frag_colour;
			layout (location = 1) out int io_entity;

			uniform int u_entity;
			uniform vec4 u_colour;
			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour = texture(u_texture, io_texels) * u_colour;
				io_entity = u_entity;
			}
		)";

		///
		/// Video vertex shader.
		///
		constexpr const auto video_vert = R"(
			#version 460 core
			
			layout(location = 0) in vec2 vertex;
			
			out vec2 io_texels;
			
			void main()
			{
				io_texels = vertex;
				gl_Position = vec4((vertex * 2.0 - 1.0) * vec2(1, -1), 0.0, 1.0);
			}
		)";

		///
		/// Video frag shader.
		///
		constexpr const auto video_frag = R"(
			#version 460 core
			
			in vec2 io_texels;
			out vec4 io_frag;
			
			uniform sampler2D u_texture_y;
			uniform sampler2D u_texture_cb;
			uniform sampler2D u_texture_cr;
			
			mat4 rec601 = mat4(
				1.16438,  0.00000,  1.59603, -0.87079,
				1.16438, -0.39176, -0.81297,  0.52959,
				1.16438,  2.01723,  0.00000, -1.08139,
				0, 0, 0, 1
			);
			
			void main()
			{
				float y = texture2D(texture_y, tex_coord).r;
				float cb = texture2D(texture_cb, tex_coord).r;
				float cr = texture2D(texture_cr, tex_coord).r;

				io_frag = vec4(y, cb, cr, 1.0) * rec601;
			}
		)";
	} // namespace shaders
} // namespace galaxy

#endif