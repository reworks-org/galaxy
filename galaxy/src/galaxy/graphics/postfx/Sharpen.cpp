///
/// Sharpen.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Sharpen.hpp"

///
/// Sharpen vertex shader.
///
inline constexpr const char* const sharpen_vert = R"(
	#version 450 core
	layout (location = 0) in vec3 l_pos;
	layout (location = 1) in vec2 l_texels;

	void main()
	{
		gl_Position = vec4(l_pos, 1.0);
	}
)";

///
/// Sharpen fragment shader.
///
inline constexpr const char* const sharpen_frag = R"(
	/*
	  (C) 2019 David Lettier
	  lettier.com
	*/

	#version 450 core

	out vec4 fragColor;

	uniform sampler2D colorTexture;

	void main()
	{
	  float amount = 0.3;

	  vec2 texSize   = textureSize(colorTexture, 0).xy;
	  vec2 fragCoord = gl_FragCoord.xy;
	  vec2 texCoord  = fragCoord / texSize;

	  float neighbor = amount * -1.0;
	  float center   = amount *  4.0 + 1.0;

	  vec3 color =
			texture(colorTexture, (fragCoord + vec2( 0,  1)) / texSize).rgb
		  * neighbor

		  + texture(colorTexture, (fragCoord + vec2(-1,  0)) / texSize).rgb
		  * neighbor
		  + texture(colorTexture, (fragCoord + vec2( 0,  0)) / texSize).rgb
		  * center
		  + texture(colorTexture, (fragCoord + vec2( 1,  0)) / texSize).rgb
		  * neighbor

		  + texture(colorTexture, (fragCoord + vec2( 0, -1)) / texSize).rgb
		  * neighbor
	  ;

	  fragColor = vec4(color, texture(colorTexture, texCoord).a);
	}
)";

namespace galaxy
{
	namespace graphics
	{
		Sharpen::Sharpen()
		    : PostEffect {}
		{
			m_shader.load_raw(sharpen_vert, sharpen_frag);
			m_shader.bind();
			m_shader.set_uniform("colorTexture", 0);
			m_shader.unbind();
		}

		void Sharpen::resize(const int width, const int height)
		{
			m_fb.resize(width, height);
		}

		const unsigned int Sharpen::render(const unsigned int input)
		{
			m_fb.bind();
			m_shader.bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, input);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			return m_fb.gl_texture();
		}
	} // namespace graphics
} // namespace galaxy