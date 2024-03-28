///
/// GammaCorrection.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "GammaCorrection.hpp"

///
/// Basic vertex shader.
///
constexpr const char* const gammacorrection_vert = R"(
	#version 460 core
	layout (location = 0) in vec2 l_pos;
	layout (location = 1) in vec2 l_texels;

	void main()
	{
		gl_Position = vec4(l_pos, 0.0, 1.0);
	}
)";

///
/// \brief Gamma Correction fragment shader.
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
constexpr const char* const gammacorrection_frag = R"(
	#version 460 core

	out vec4 io_frag_colour;

	uniform float u_gamma;
	uniform sampler2D u_texture;

	void main()
	{
		vec2 uv = gl_FragCoord.xy / textureSize(u_texture, 0).xy;

		io_frag_colour = texture(u_texture, uv);
		io_frag_colour.rgb = pow(io_frag_colour.rgb, vec3(1.0 / u_gamma));
	}
)";

namespace galaxy
{
	namespace graphics
	{
		GammaCorrection::GammaCorrection(const int width, const int height)
			: m_gamma {2.2f}
		{
			m_fb.create(width, height);

			if (m_shader.parse(gammacorrection_vert, gammacorrection_frag))
			{
				m_shader.compile();
				m_shader.set_uniform("u_texture", 0);
				m_shader.set_uniform("u_gamma", m_gamma);
			}
		}

		void GammaCorrection::resize(const int width, const int height)
		{
			m_fb.recreate(width, height);
		}

		unsigned int GammaCorrection::render(const unsigned int input)
		{
			m_fb.bind(true);
			m_shader.bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, input);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			return m_fb.texture();
		}

		void GammaCorrection::set_gamma(const float gamma_mod)
		{
			m_gamma = gamma_mod;

			if (static_cast<int>(m_gamma) == 0)
			{
				m_gamma = 1.0f;
			}

			m_shader.set_uniform("u_gamma", m_gamma);
		}

		float GammaCorrection::get_gamma() const
		{
			return m_gamma;
		}

		bool GammaCorrection::is_enabled()
		{
			return core::ServiceLocator<core::Config>::ref().get<bool>("gamma_correction", "graphics.effects");
		}
	} // namespace graphics
} // namespace galaxy
