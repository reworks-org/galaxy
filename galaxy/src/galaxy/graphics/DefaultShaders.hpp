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
		/// Basic vertex shader for the 2d renderer.
		///
		constexpr const auto r2d_vert = R"(
			#version 460 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;
			layout(location = 2) in vec2 l_normals;

			precision highp int;
			precision highp float;

			layout(std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_model_view;
				mat4 u_camera_proj;
			};

			layout(std140, binding = 1) uniform render_data
			{
				mat4 u_transform;
				vec4 u_colour;
				int u_entity;
				bool u_point;
				bool u_textured;
				bool u_normal_mapped;
			};

			out vec2 io_texels;
			out vec2 io_normals;
	
			void main()
			{
				gl_Position =  u_camera_proj * u_camera_model_view * u_transform * vec4(l_pos, 0.0, 1.0);

				io_texels = l_texels;
				io_texels.y = 1.0 - io_texels.y;

				io_normals = l_normals;
				io_normals.y = 1.0 - io_normals.y;
	
				if (u_point)
				{
					gl_PointSize = 4;
				}
			}
		)";

		///
		/// Basic fragment shader for the 2d renderer.
		///
		constexpr const auto r2d_frag = R"(
			#version 460 core

			precision highp int;
			precision highp float;

			#define STEP_A 0.4
			#define STEP_B 0.6
			#define STEP_C 0.8
			#define STEP_D 1.0

			struct Light
            {
	            vec4 colour;
				vec3 falloff;
				vec2 pos;
				float depth;
				float diameter;
            };

			layout(std140, binding = 0) uniform camera_data
			{
				mat4 u_camera_model_view;
				mat4 u_camera_proj;
			};

			layout(std140, binding = 1) uniform render_data
			{
				mat4 u_transform;
				vec4 u_colour;
				int u_entity;
				bool u_point;
				bool u_textured;
				bool u_normal_mapped;
			};

			layout(std430, binding = 0) readonly buffer light_data {
				vec2 s_resolution;
				vec4 s_ambient_colour;
				Light s_lights[];
			};
			
			layout (location = 0) out vec4 io_frag_colour;
			layout (location = 1) out int io_entity;

			in vec2 io_texels;
			in vec2 io_normals;

			layout (location = 0) uniform sampler2D u_texture;
			layout (location = 1) uniform sampler2D u_normals;

			void main()
			{
				if (u_textured)
				{
					vec3 diffuse_colour = texture(u_texture, io_texels).rgb * u_colour.rgb;
					vec3 sum = vec3(0.0);

					if (u_normal_mapped)
					{
						vec3 normals = texture(u_normals, io_normals).rgb;
						for (int i = 0; i < s_lights.length(); i++)
						{
							Light light = s_lights[i];
							
							vec3 light_dir = vec3(light.pos - (gl_FragCoord.xy / s_resolution), light.depth);
							light_dir.x /= (light.diameter / s_resolution.x);
							light_dir.y /= (light.diameter / s_resolution.y);

							float D = length(light_dir);
							
							vec3 N = normalize(normals * 2.0 - 1.0);
							vec3 L = normalize(light_dir);

							N = mix(N, vec3(0), 0.5);

							vec3 diffuse = (light.colour.xyz * light.colour.w) * max(dot(N, L), 0.0);
							vec3 ambient = s_ambient_colour.xyz * s_ambient_colour.w;
							
							float attenuation = 1.0 / (light.falloff.x + (light.falloff.y * D) + (light.falloff.z * D * D));
							if (attenuation < STEP_A) 
								attenuation = 0.0;
							else if (attenuation < STEP_B) 
								attenuation = STEP_B;
							else if (attenuation < STEP_C) 
								attenuation = STEP_C;
							else 
								attenuation = STEP_D;

							sum += (diffuse_colour * (ambient + diffuse * attenuation));
						}
					}
					else
					{
						sum = diffuse_colour;
					}
					
					io_frag_colour = vec4(sum, u_colour.a);
				}
				else
				{
					io_frag_colour = u_colour;
				}

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