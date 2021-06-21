///
/// PostProcessor.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "PostProcessor.hpp"

///
/// Plain VAO vertex shader.
///
constexpr const char* const vao_vert = R"(
	#version 450 core
	layout (location = 0) in vec2 l_pos;
	layout (location = 1) in vec2 l_texels;
	
	out vec2 io_texels;
	
	void main()
	{
		io_texels = l_texels;
		gl_Position = vec4(l_pos, 0.0, 1.0);
	}
)";

///
/// Plain VAO fragment shader.
///
constexpr const char* const vao_frag = R"(
	#version 450 core

	in vec2 io_texels;

	out vec4 io_frag_colour;
	
	uniform sampler2D u_texture;

	void main()
	{
		io_frag_colour = texture(u_texture, io_texels);
	}
)";

namespace galaxy
{
	namespace graphics
	{
		PostProcessor::PostProcessor()
		    : m_screen_vbo {0}, m_screen_vao {0}, m_output_fb {0}
		{
			glGenBuffers(1, &m_screen_vbo);
			glGenVertexArrays(1, &m_screen_vao);

			m_output.load_raw(vao_vert, vao_frag);

			// clang-format off
			constexpr const std::array<float, 16> verticies =
			{
				// First 3 are pos, last 2 are texels.
				0.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 0.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				0.0f, 1.0f, 0.0f, 1.0f
			};
			// clang-format on

			glBindVertexArray(m_screen_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_screen_vbo);
			glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

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

			// Post-processing effects pass.
			m_output_fb = m_smaa.render(m_fb.get_texture());
			m_output_fb = m_sharpen.render(m_output_fb);

			// Final Output.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, SL_HANDLE.window()->get_width(), SL_HANDLE.window()->get_height());

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_output.bind();

			glBindTexture(GL_TEXTURE_2D, m_output_fb);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}
	} // namespace graphics
} // namespace galaxy