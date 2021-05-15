///
/// PostProcessor.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/graphics/shaders/post/SMAA.hpp"

#include "PostProcessor.hpp"

namespace galaxy
{
	namespace graphics
	{
		PostProcessor::PostProcessor()
		    : m_width {0},
		      m_height {0},
		      m_screen_vbo {0},
		      m_screen_vao {0},
		      m_edge_tex {0},
		      m_blend_tex {0},
		      m_area_tex {0},
		      m_search_tex {0},
		      m_edge_fbo {0},
		      m_blend_fbo {0}
		{
			glGenBuffers(1, &m_screen_vbo);
			glGenVertexArrays(1, &m_screen_vao);

			glGenFramebuffers(1, &m_edge_fbo);
			glGenFramebuffers(1, &m_blend_fbo);

			glGenTextures(1, &m_edge_tex);
			glGenTextures(1, &m_blend_tex);
			glGenTextures(1, &m_area_tex);
			glGenTextures(1, &m_search_tex);
		}

		PostProcessor::~PostProcessor()
		{
			glDeleteVertexArrays(1, &m_screen_vao);
			glDeleteBuffers(1, &m_screen_vbo);

			glDeleteFramebuffers(1, &m_blend_fbo);
			glDeleteFramebuffers(1, &m_edge_fbo);

			glDeleteTextures(1, &m_search_tex);
			glDeleteTextures(1, &m_area_tex);
			glDeleteTextures(1, &m_blend_tex);
			glDeleteTextures(1, &m_edge_tex);
		}

		void PostProcessor::init(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

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

			m_fb.create(m_width, m_height);

			init_smaa();
		}

		void PostProcessor::resize(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			m_fb.resize(m_width, m_height);

			glBindTexture(GL_TEXTURE_2D, m_edge_tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);

			glBindTexture(GL_TEXTURE_2D, m_blend_tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);

			glBindTexture(GL_TEXTURE_2D, 0);
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

			// SMAA Edge Pass.
			glBindFramebuffer(GL_FRAMEBUFFER, m_edge_fbo);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_smaa_edge.bind();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_fb.gl_texture());
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			// SMAA Blending Pass.
			glBindFramebuffer(GL_FRAMEBUFFER, m_blend_fbo);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_smaa_blend.bind();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_edge_tex);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_area_tex);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_search_tex);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			// SMAA Neighbourhood Pass.
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			m_smaa_neighbourhood.bind();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_fb.gl_texture());
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_blend_tex);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			// SMAA END.
			glUseProgram(0);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void PostProcessor::init_smaa()
		{
			constexpr const std::array<unsigned int, 1> attachment = {GL_COLOR_ATTACHMENT0};

			glBindFramebuffer(GL_FRAMEBUFFER, m_edge_fbo);

			glBindTexture(GL_TEXTURE_2D, m_edge_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_edge_tex, 0);
			glDrawBuffers(attachment.size(), attachment.data());

			glBindFramebuffer(GL_FRAMEBUFFER, m_blend_fbo);

			glBindTexture(GL_TEXTURE_2D, m_blend_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blend_tex, 0);
			glDrawBuffers(attachment.size(), attachment.data());

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glBindTexture(GL_TEXTURE_2D, m_area_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, AREATEX_WIDTH, AREATEX_HEIGHT, 0, GL_RG, GL_UNSIGNED_BYTE, shaders::smaa_area_tex);

			glBindTexture(GL_TEXTURE_2D, m_search_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, SEARCHTEX_WIDTH, SEARCHTEX_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, shaders::smaa_search_tex);

			glBindTexture(GL_TEXTURE_2D, 0);

			const auto smaa_full = std::format("{0}{1}{2}{3}", shaders::smaa_core_part1, shaders::smaa_core_part2, shaders::smaa_core_part3, shaders::smaa_core_part4);

			const auto edge_vert = std::format("{0}{1}{2}", shaders::smaa_header_vert, smaa_full, shaders::smaa_edge_vert);
			const auto edge_frag = std::format("{0}{1}{2}", shaders::smaa_header_frag, smaa_full, shaders::smaa_edge_frag);
			m_smaa_edge.load_raw(edge_vert, edge_frag);
			m_smaa_edge.bind();
			m_smaa_edge.set_uniform("albedo_tex", 0);

			const auto blend_vert = std::format("{0}{1}{2}", shaders::smaa_header_vert, smaa_full, shaders::smaa_blend_vert);
			const auto blend_frag = std::format("{0}{1}{2}", shaders::smaa_header_frag, smaa_full, shaders::smaa_blend_frag);
			m_smaa_blend.load_raw(blend_vert, blend_frag);
			m_smaa_blend.bind();
			m_smaa_blend.set_uniform("edge_tex", 0);
			m_smaa_blend.set_uniform("area_tex", 1);
			m_smaa_blend.set_uniform("search_tex", 2);

			const auto neighbourhood_vert = std::format("{0}{1}{2}", shaders::smaa_header_vert, smaa_full, shaders::smaa_neighbour_vert);
			const auto neighbourhood_frag = std::format("{0}{1}{2}", shaders::smaa_header_frag, smaa_full, shaders::smaa_neighbour_frag);
			m_smaa_neighbourhood.load_raw(neighbourhood_vert, neighbourhood_frag);
			m_smaa_neighbourhood.bind();
			m_smaa_neighbourhood.set_uniform("albedo_tex", 0);
			m_smaa_neighbourhood.set_uniform("blend_tex", 1);
		}
	} // namespace graphics
} // namespace galaxy