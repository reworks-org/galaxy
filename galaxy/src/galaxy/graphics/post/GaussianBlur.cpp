///
/// GaussianBlur.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "GaussianBlur.hpp"

///
/// Basic vertex shader.
///
constexpr const char* const gaussianblur_vert = R"(
	#version 460 core
	layout (location = 0) in vec2 l_pos;
	layout (location = 1) in vec2 l_texels;

	void main()
	{
		gl_Position = vec4(l_pos, 0.0, 1.0);
	}
)";

/*
The MIT License (MIT) Copyright (c) 2015 Jam3

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR
A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
constexpr const char* const gaussianblur_frag = R"(
	#version 460 core

	out vec4 io_frag_colour;

	uniform int u_strength;
	uniform vec2 u_direction;
	uniform sampler2D u_texture;

	vec4 blur5(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
		vec4 color = vec4(0.0);
		vec2 off1 = vec2(1.3333333333333333) * direction;
	
		color += texture2D(image, uv) * 0.29411764705882354;
		color += texture2D(image, uv + (off1 / resolution)) * 0.35294117647058826;
		color += texture2D(image, uv - (off1 / resolution)) * 0.35294117647058826;
	
		return color; 
	}

	vec4 blur9(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
		vec4 color = vec4(0.0);
		vec2 off1 = vec2(1.3846153846) * direction;
		vec2 off2 = vec2(3.2307692308) * direction;
		
		color += texture2D(image, uv) * 0.2270270270;
		color += texture2D(image, uv + (off1 / resolution)) * 0.3162162162;
		color += texture2D(image, uv - (off1 / resolution)) * 0.3162162162;
		color += texture2D(image, uv + (off2 / resolution)) * 0.0702702703;
		color += texture2D(image, uv - (off2 / resolution)) * 0.0702702703;
		
		return color;
	}

	vec4 blur13(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
		vec4 color = vec4(0.0);
		vec2 off1 = vec2(1.411764705882353) * direction;
		vec2 off2 = vec2(3.2941176470588234) * direction;
		vec2 off3 = vec2(5.176470588235294) * direction;

		color += texture2D(image, uv) * 0.1964825501511404;
		color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
		color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
		color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
		color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
		color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
		color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;

		return color;
	}

	void main()
	{
		vec2 size = textureSize(u_texture, 0).xy;
		vec2 uv = gl_FragCoord.xy / size.xy;

		if (u_strength == 0)
		{
			io_frag_colour = blur5(u_texture, uv, size, u_direction);
		}
		else if (u_strength == 1)
		{
			io_frag_colour = blur9(u_texture, uv, size, u_direction);
		}
		else if (u_strength == 2)
		{
			io_frag_colour = blur13(u_texture, uv, size, u_direction);
		}
	}
)";

namespace galaxy
{
	namespace graphics
	{
		GaussianBlur::GaussianBlur(const int width, const int height)
			: m_strength {Strength::NORMAL}
		{
			m_horizontal.create(width, height);
			m_vertical.create(width, height);

			if (m_shader.parse(gaussianblur_vert, gaussianblur_frag))
			{
				m_shader.compile();
				m_shader.set_uniform("u_texture", 0);
				m_shader.set_uniform("u_strength", static_cast<int>(m_strength));
			}
		}

		void GaussianBlur::resize(const int width, const int height)
		{
			m_horizontal.recreate(width, height);
			m_vertical.recreate(width, height);
		}

		unsigned int GaussianBlur::render(const unsigned int input)
		{
			m_shader.bind();

			m_horizontal.bind(true);
			m_shader.set_uniform("u_direction", 1.0f, 0.0f);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, input);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			m_vertical.bind(true);
			m_shader.set_uniform("u_direction", 0.0f, 1.0f);

			glBindTexture(GL_TEXTURE_2D, m_horizontal.texture());
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			return m_vertical.texture();
		}

		void GaussianBlur::set_strength(const Strength strength)
		{
			m_strength = strength;
			m_shader.set_uniform("u_strength", static_cast<int>(m_strength));
		}

		bool GaussianBlur::is_enabled()
		{
			return core::ServiceLocator<core::Config>::ref().get<bool>("gaussian_blur", "graphics.effects");
		}
	} // namespace graphics
} // namespace galaxy
