///
/// ChromaticAberration.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "ChromaticAberration.hpp"

///
/// Basic vertex shader.
///
constexpr const char* const chromaticaberration_vert = R"(
	#version 460 core
	layout (location = 0) in vec2 l_pos;
	layout (location = 1) in vec2 l_texels;

	void main()
	{
		gl_Position = vec4(l_pos, 0.0, 1.0);
	}
)";

/*
BSD 3-Clause License

Copyright (c) 2019, David Lettier
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
constexpr const char* const chromaticaberration_frag = R"(
	#version 460 core

	out vec4 io_frag_colour;

	uniform vec2 u_direction;
	uniform vec3 u_rgb_offset;
	uniform sampler2D u_texture;

	void main()
	{
		vec2 texSize = textureSize(u_texture, 0).xy;
	    vec2 texCoord = gl_FragCoord.xy / texSize;

		vec2 direction = texCoord - u_direction;

		io_frag_colour = texture(u_texture, texCoord);
		io_frag_colour.r = texture(u_texture, texCoord + (direction * vec2(u_rgb_offset.x))).r;
		io_frag_colour.g = texture(u_texture, texCoord + (direction * vec2(u_rgb_offset.y))).g;
		io_frag_colour.b = texture(u_texture, texCoord + (direction * vec2(u_rgb_offset.z))).b;
	}
)";

namespace galaxy
{
	namespace graphics
	{
		ChromaticAberration::ChromaticAberration(const int width, const int height) noexcept
			: m_r_offset {0.009f}
			, m_g_offset {0.006f}
			, m_b_offset {-0.006f}
		{
			m_fb.create(width, height);

			m_shader.load_raw(chromaticaberration_vert, chromaticaberration_frag);
			m_shader.compile();

			m_shader.set_uniform("u_texture", 0);
			m_shader.set_uniform("u_rgb_offset", m_r_offset, m_g_offset, m_b_offset);
			m_shader.set_uniform("u_direction", 0.509167f, 0.598f); // may need tweaking, in general 0.5, 0.5 is default, adjust to change direction.
		}

		void ChromaticAberration::resize(const int width, const int height)
		{
			m_fb.resize(width, height);
		}

		unsigned int ChromaticAberration::render(const unsigned int input)
		{
			m_fb.bind(true);
			m_shader.bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, input);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			return m_fb.get_texture();
		}

		void ChromaticAberration::set_r_offset(const float r) noexcept
		{
			m_r_offset = r;
			m_shader.set_uniform("u_rgb_offset", m_r_offset, m_g_offset, m_b_offset);
		}

		void ChromaticAberration::set_g_offset(const float g) noexcept
		{
			m_g_offset = g;
			m_shader.set_uniform("u_rgb_offset", m_r_offset, m_g_offset, m_b_offset);
		}

		void ChromaticAberration::set_b_offset(const float b) noexcept
		{
			m_b_offset = b;
			m_shader.set_uniform("u_rgb_offset", m_r_offset, m_g_offset, m_b_offset);
		}

		void ChromaticAberration::set_rgb_offset(const float r, const float g, const float b) noexcept
		{
			m_r_offset = r;
			m_g_offset = g;
			m_b_offset = b;

			m_shader.set_uniform("u_rgb_offset", m_r_offset, m_g_offset, m_b_offset);
		}
	} // namespace graphics
} // namespace galaxy