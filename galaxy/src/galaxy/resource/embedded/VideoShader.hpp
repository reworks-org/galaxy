///
/// VideoShader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_EMBEDDED_VIDEOSHADER_HPP_
#define GALAXY_RESOURCE_EMBEDDED_VIDEOSHADER_HPP_

namespace galaxy
{
	namespace resource
	{
		///
		/// Video vertex shader.
		///
		constexpr const auto video_vert_shader = R"(
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
		constexpr const auto video_frag_shader = R"(
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
	} // namespace resource
} // namespace galaxy

#endif
