///
/// PostProcess.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "PostProcess.hpp"

///
/// Plain VAO vertex shader.
///
constexpr const auto vao_vert = R"(
	#version 460 core

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
constexpr const auto vao_frag = R"(
	#version 460 core

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
		PostProcess::PostProcess()
			: m_screen_vbo {0}
			, m_screen_vao {0}
			, m_output_fb {0}
		{
		}

		PostProcess::~PostProcess()
		{
			destroy();
		}

		void PostProcess::init(const int width, const int height)
		{
			if (m_output.parse(vao_vert, vao_frag))
			{
				m_output.compile();
			}

			// We use the old style of opengl here, rather than DSA, just to keep it simple.

			glGenBuffers(1, &m_screen_vbo);
			glGenVertexArrays(1, &m_screen_vao);

			// clang-format off
			constexpr const std::array<float, 16> verticies =
			{
				// First 2 are pos, last 2 are texels.
				-1.0f,  1.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 1.0f, 0.0f
			};
			// clang-format on

			glBindVertexArray(m_screen_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_screen_vbo);
			glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
			glEnableVertexAttribArray(1);

			constexpr const std::uint64_t size = 2 * sizeof(float);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(size));

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			m_fb.create(width, height);
		}

		void PostProcess::destroy()
		{
			m_effects.clear();

			if (m_screen_vao != 0)
			{
				glDeleteVertexArrays(1, &m_screen_vao);
				m_screen_vao = 0;
			}

			if (m_screen_vbo != 0)
			{
				glDeleteBuffers(1, &m_screen_vbo);
				m_screen_vbo = 0;
			}

			m_output_fb = 0;
		}

		void PostProcess::resize(const int width, const int height)
		{
			m_fb.recreate(width, height);

			for (auto&& effect : m_effects)
			{
				effect->resize(width, height);
			}
		}

		void PostProcess::bind()
		{
			m_fb.bind(true);
		}

		void PostProcess::unbind()
		{
			m_fb.unbind();
		}

		void PostProcess::render_effects()
		{
			glBindVertexArray(m_screen_vao);

			m_output_fb = m_fb.texture();

			// Post-processing effects pass.
			for (auto&& effect : m_effects)
			{
				if (effect->is_enabled())
				{
					m_output_fb = effect->render(m_output_fb);
				}
			}
		}

		void PostProcess::render_output()
		{
			m_output.bind();

			glBindVertexArray(m_screen_vao);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_output_fb);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	} // namespace graphics
} // namespace galaxy
