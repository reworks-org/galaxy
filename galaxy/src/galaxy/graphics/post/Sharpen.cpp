///
/// Sharpen.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include <glad/glad.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "Sharpen.hpp"

///
/// Basic vertex shader.
///
constexpr const char* const sharpen_vert = R"(
	#version 460 core
	layout (location = 0) in vec2 l_pos;
	layout (location = 1) in vec2 l_texels;

	void main()
	{
		gl_Position = vec4(l_pos, 0.0, 1.0);
	}
)";

///
/// \brief Sharpen fragment shader.
///
/// BSD 3-Clause License
///
/// Copyright (c) 2019, David Lettier
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// * Redistributions of source code must retain the above copyright notice, this
/// list of conditions and the following disclaimer.
///
/// * Redistributions in binary form must reproduce the above copyright notice,
/// this list of conditions and the following disclaimer in the documentation
/// and/or other materials provided with the distribution.
///
/// * Neither the name of the copyright holder nor the names of its
/// contributors may be used to endorse or promote products derived from
/// this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
/// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
constexpr const char* const sharpen_frag = R"(
	#version 460 core

	out vec4 io_frag_colour;

	uniform float u_amount;
	uniform sampler2D u_texture;

	void main()
	{
		vec2 tex_size = textureSize(u_texture, 0).xy;
        vec2 frag_coord = gl_FragCoord.xy;
		vec2 tex_coord = frag_coord / tex_size;

        float neighbour = u_amount * -1.0;
        float center = u_amount * 4.0 + 1.0;

        vec3 sharpen =
        texture(u_texture, (frag_coord + vec2( 0,  1)) / tex_size).rgb * neighbour
        + texture(u_texture, (frag_coord + vec2(-1,  0)) / tex_size).rgb * neighbour
        + texture(u_texture, (frag_coord + vec2( 0,  0)) / tex_size).rgb * center
        + texture(u_texture, (frag_coord + vec2( 1,  0)) / tex_size).rgb * neighbour
        + texture(u_texture, (frag_coord + vec2( 0, -1)) / tex_size).rgb * neighbour;

		io_frag_colour = vec4(sharpen, texture(u_texture, tex_coord).a);
	}
)";

namespace galaxy
{
	namespace graphics
	{
		Sharpen::Sharpen(const int width, const int height)
			: m_amount {0.1f}
		{
			m_fb.create(width, height);

			if (m_shader.parse(sharpen_vert, sharpen_frag))
			{
				m_shader.compile();
				m_shader.set_uniform("u_texture", 0);
				m_shader.set_uniform("u_amount", m_amount);
			}
		}

		void Sharpen::resize(const int width, const int height)
		{
			m_fb.recreate(width, height);
		}

		unsigned int Sharpen::render(const unsigned int input)
		{
			m_fb.bind(true);
			m_shader.bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, input);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			return m_fb.texture();
		}

		void Sharpen::set_amount(const float amount)
		{
			m_amount = std::clamp(amount, 0.0f, 10.0f);
			m_shader.set_uniform("u_amount", m_amount);
		}

		float Sharpen::get_amount() const
		{
			return m_amount;
		}

		bool Sharpen::is_enabled()
		{
			return core::ServiceLocator<core::Config>::ref().get<bool>("sharpen", "graphics.effects");
		}
	} // namespace graphics
} // namespace galaxy
