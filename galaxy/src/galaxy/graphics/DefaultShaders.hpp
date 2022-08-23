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
		static inline constexpr const auto sprite_vert = R"(
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
				io_texels = l_texels;
				gl_Position =  u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);
			}
		)";

		///
		/// Sprite fragment shader.
		///
		static inline constexpr const auto sprite_frag = R"(
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
		/// Point vertex shader.
		///
		static inline constexpr const auto point_vert = R"(
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
		static inline constexpr const auto point_frag = R"(
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
		static inline constexpr const auto line_vert = R"(
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
		static inline constexpr const auto line_frag = R"(
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
		static inline constexpr const auto render_to_texture_vert = R"(
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
			}
		)";

		///
		/// Render To Texture fragment shader.
		///
		static inline constexpr const auto render_to_texture_frag = R"(
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
		/* static inline constexpr const auto text_vert = R"(
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
				io_texels.x = (((l_texels.x - 0.0) * (1.0 - 0.0)) / (u_width - 0.0)) + 0.0;
				io_texels.y = 1.0 - (((l_texels.y - 0.0) * (1.0 - 0.0)) / (u_height - 0.0)) + 0.0;

				gl_Position =  u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);
			}
		)";

		///
		/// Text fragment shader.
		///
		static inline constexpr const auto text_frag = R"(
			#version 460 core

			in vec2 io_texels;
			out vec4 io_frag_colour;

			uniform vec4 u_colour;
			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour.rgb = u_colour.rgb;
				io_frag_colour.a = texture(u_texture, io_texels).a * u_colour.a;
			}
		)";*/

		///
		/// SpriteBatch vertex shader.
		///
		/* static inline constexpr const auto spritebatch_vert = R"(
			#version 460 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;
			layout(location = 2) in vec4 l_colour;

			out vec2 io_texels;
			out float io_opacity;

			layout(std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_model_view;
				mat4 u_camera_proj;
			};

			uniform float u_width;
			uniform float u_height;

			void main()
			{
				io_texels.x = (((l_texels.x - 0.0) * (1.0 - 0.0)) / (u_width - 0.0)) + 0.0;
				io_texels.y = 1.0 - (((l_texels.y - 0.0) * (1.0 - 0.0)) / (u_height - 0.0)) + 0.0;
				io_opacity = l_colour.a;

				gl_Position =  u_camera_proj * u_camera_model_view * vec4(l_pos, 0.0, 1.0);
			}
		)";

		///
		/// SpriteBatch fragment shader.
		///
		static inline constexpr const auto spritebatch_frag = R"(
			#version 460 core

			in vec2 io_texels;
			in float io_opacity;

			out vec4 io_frag_colour;

			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour = texture(u_texture, io_texels);
				io_frag_colour.a *= io_opacity;
			}
		)";*/

		///
		/// Instance vertex shader.
		///
		/* static inline constexpr const auto instance_vert = R"(
			#version 460 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;
			layout(location = 2) in vec4 l_colour;
			layout(location = 3) in vec2 l_instance_offset;

			out vec2 io_texels;

			layout(std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_model_view;
				mat4 u_camera_proj;
			};

			uniform float u_width;
			uniform float u_height;

			void main()
			{
				io_texels.x = (((l_texels.x - 0.0) * (1.0 - 0.0)) / (u_width - 0.0)) + 0.0;
				io_texels.y = 1.0 - (((l_texels.y - 0.0) * (1.0 - 0.0)) / (u_height - 0.0)) + 0.0;

				gl_Position = u_camera_proj * u_camera_model_view * vec4(l_pos + l_instance_offset, 0.0, 1.0);
			}
		)";

		///
		/// Instance fragment shader.
		///
		static inline constexpr const auto instance_frag = R"(
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
		)";*/
	} // namespace shaders
} // namespace galaxy

#endif