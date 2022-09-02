///
/// PostProcess.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/graphics/AttributeBindings.hpp"
#include "galaxy/graphics/BufferBindings.hpp"

#include "PostProcess.hpp"

///
/// Plain VAO vertex shader.
///
constexpr const char* const vao_vert = R"(
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
constexpr const char* const vao_frag = R"(
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
		PostProcess::PostProcess(const int width, const int height) noexcept
			: m_screen_vbo {0}
			, m_screen_vao {0}
			, m_output_fb {0}
		{
			m_output.load_raw(vao_vert, vao_frag);
			m_output.compile();

			// clang-format off
			constexpr const std::array<float, 16> verticies = {
				// First 2 are pos, last 2 are texels.
				-1.0f, 1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f,
				1.0f,  1.0f,  1.0f, 1.0f,
				1.0f,  -1.0f, 1.0f, 0.0f
			};
			// clang-format on

			glCreateBuffers(1, &m_screen_vbo);
			glCreateVertexArrays(1, &m_screen_vao);

			glNamedBufferStorage(m_screen_vbo, verticies.size(), verticies.data(), GL_DYNAMIC_STORAGE_BIT);

			glVertexArrayVertexBuffer(m_screen_vao, static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT), m_screen_vbo, 0, 4 * sizeof(float));

			glEnableVertexArrayAttrib(m_screen_vao, static_cast<unsigned int>(AttributeBinding::POSITION_POINT)); // Pos
			glEnableVertexArrayAttrib(m_screen_vao, static_cast<unsigned int>(AttributeBinding::TEXEL_POINT));    // Texels

			glVertexArrayAttribFormat(m_screen_vao, static_cast<unsigned int>(AttributeBinding::POSITION_POINT), 2, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribFormat(m_screen_vao, static_cast<unsigned int>(AttributeBinding::TEXEL_POINT), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));

			glVertexArrayAttribBinding(m_screen_vao,
				static_cast<unsigned int>(AttributeBinding::POSITION_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));
			glVertexArrayAttribBinding(m_screen_vao,
				static_cast<unsigned int>(AttributeBinding::TEXEL_POINT),
				static_cast<unsigned int>(BufferBinding::VERTEX_BUFFER_POINT));

			m_fb.create(width, height);
		}

		PostProcess::~PostProcess()
		{
			glDeleteVertexArrays(1, &m_screen_vao);
			glDeleteBuffers(1, &m_screen_vbo);
		}

		void PostProcess::resize(const int width, const int height)
		{
			m_fb.resize(width, height);

			for (auto&& effect : m_effects)
			{
				effect->resize(width, height);
			}
		}

		void PostProcess::bind() noexcept
		{
			m_fb.bind(true);
		}

		void PostProcess::unbind() noexcept
		{
			m_fb.unbind();
		}

		void PostProcess::render_effects()
		{
			glBindVertexArray(m_screen_vao);

			m_output_fb = m_fb.get_texture();

			// Post-processing effects pass.
			for (auto&& effect : m_effects)
			{
				m_output_fb = effect->render(m_output_fb);
			}
		}

		void PostProcess::render_output()
		{
			m_output.bind();

			glBindTexture(GL_TEXTURE_2D, m_output_fb);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	} // namespace graphics
} // namespace galaxy