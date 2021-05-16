///
/// PostProcessor.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/shaders/VAO.hpp"

#include "PostProcessor.hpp"

namespace galaxy
{
	namespace graphics
	{
		PostProcessor::PostProcessor()
		    : m_screen_vbo {0},
		      m_screen_vao {0}
		{
			glGenBuffers(1, &m_screen_vbo);
			glGenVertexArrays(1, &m_screen_vao);

			m_output.load_raw(shaders::vao_vert, shaders::vao_frag);
			m_output.bind();
			m_output.set_uniform("u_texture", 0);
			m_output.unbind();

			// clang-format off
			constexpr const std::array<float, 20> verticies =
			{
				// First 3 are Pos, last 2 are Texels.
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
			};
			// clang-format on

			glBindVertexArray(m_screen_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_screen_vbo);
			glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			m_fb.create(SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height());
		}

		PostProcessor::~PostProcessor()
		{
			glDeleteVertexArrays(1, &m_screen_vao);
			glDeleteBuffers(1, &m_screen_vbo);
		}

		void PostProcessor::resize(const int width, const int height)
		{
			m_fb.resize(width, height);
			m_smaa.resize(width, height);
			m_sharpen.resize(width, height);
		}

		void PostProcessor::bind() noexcept
		{
			m_fb.bind();
		}

		void PostProcessor::unbind() noexcept
		{
			m_fb.unbind();
		}

		void PostProcessor::render()
		{
			glBindVertexArray(m_screen_vao);

			unsigned int output = 0;
			output              = m_smaa.render(m_fb.gl_texture());
			output              = m_sharpen.render(output);

			// Final Output.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			m_output.bind();

			glBindTexture(GL_TEXTURE_2D, output);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}
	} // namespace graphics
} // namespace galaxy