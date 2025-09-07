///
/// FilmicGrain.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "FilmicGrain.hpp"

///
/// Basic vertex shader.
///
constexpr const char* const filmicgrain_vert = R"(
	#version 460 core
	layout (location = 0) in vec2 l_pos;
	layout (location = 1) in vec2 l_texels;

	void main()
	{
		gl_Position = vec4(l_pos, 0.0, 1.0);
	}
)";

///
/// \brief Filmic Grain fragment shader.
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
constexpr const char* const filmicgrain_frag = R"(
	#version 460 core
	#define PI 3.1415926535897932384626433832795
	
	out vec4 io_frag_colour;
	
	uniform float u_amount;
	uniform float u_time;
	uniform sampler2D u_texture;
	
	void main()
	{
		vec2 tex_size = textureSize(u_texture, 0).xy;
	    vec2 tex_coord = gl_FragCoord.xy / tex_size;

		vec4 colour = texture(u_texture, tex_coord);

		float intensity = fract(10000 * sin((gl_FragCoord.x + gl_FragCoord.y * u_time) * PI));

		colour.rgb += (u_amount * intensity);
		
		io_frag_colour = colour;
	}
)";

namespace galaxy
{
	namespace graphics
	{
		FilmicGrain::FilmicGrain(const int width, const int height)
			: m_amount {0.01}
		{
			m_fb.create(width, height);

			if (m_shader.parse(filmicgrain_vert, filmicgrain_frag))
			{
				m_shader.compile();
			}

			m_shader.set_uniform("u_texture", 0);
			m_shader.set_uniform("u_amount", m_amount);
			m_shader.set_uniform("u_time", static_cast<float>(glfwGetTime()));
		}

		void FilmicGrain::resize(const int width, const int height)
		{
			m_fb.recreate(width, height);
		}

		unsigned int FilmicGrain::render(const unsigned int input)
		{
			m_fb.bind(true);
			m_shader.bind();

			m_shader.set_uniform("u_time", static_cast<float>(glfwGetTime()));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, input);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			return m_fb.texture();
		}

		void FilmicGrain::set_amount(const float amount)
		{
			m_amount = amount;
			m_shader.set_uniform("u_amount", m_amount);
		}

		bool FilmicGrain::is_enabled()
		{
			return core::ServiceLocator<core::Config>::ref().get<bool>("film_grain", "graphics.effects");
		}
	} // namespace graphics
} // namespace galaxy
