///
/// RenderToTexture.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERTOTEXTURE_HPP_
#define QUASAR_RENDERTOTEXTURE_HPP_

#include <string>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Vertex shader for drawing RenderToTextures.
	///
	static inline std::string s_renderToTextureVS = R""(
		#version 450 core
		layout(location = 0) in vec2 l_pos;
		layout(location = 1) in vec4 l_colour;
		layout(location = 2) in vec2 l_texels;

		out vec2 io_texels;

		uniform mat4 u_projection;
		uniform mat4 u_transform;
		uniform float u_width;
		uniform float u_height;

		void main()
		{
			gl_Position = u_projection * u_transform * vec4(l_pos, 0.0, 1.0);
			io_texels = vec2(l_texels.x / u_width, 1.0 - (l_texels.y / u_height));
		}
)"";

	///
	/// Fragment shader for drawing RenderToTextures.
	///
	static inline std::string s_renderToTextureFS = R""(
		#version 450 core

		in vec2 io_texels;

		out vec4 io_frag_colour;

		uniform sampler2D u_texture;

		void main()
		{
			io_frag_colour = texture(u_texture, io_texels);
		}
)"";
}

#endif