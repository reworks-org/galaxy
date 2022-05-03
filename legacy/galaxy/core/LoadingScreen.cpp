///
/// LoadingScreen.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <thread>

#include <glfw/glfw3.h>

#include "LoadingScreen.hpp"

///
/// Logo vertex shader.
///
constexpr const char* const logo_vert = R"(
	#version 460 core
	layout(location = 0) in vec2 l_pos;
	layout(location = 1) in vec2 l_texels;

	out vec2 io_texels;

	uniform mat4 u_proj;

	void main()
	{
		io_texels = l_texels;
		gl_Position =  u_proj * vec4(l_pos, 0.0, 1.0);
	}
)";

///
/// Logo fragment shader.
///
constexpr const char* const logo_frag = R"(
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
	namespace core
	{
		LoadingScreen::LoadingScreen() noexcept
		{
			m_shader.load_raw(logo_vert, logo_frag);
		}

		void LoadingScreen::load(std::string_view texture)
		{
			m_texture.load(texture);
			m_texture.set_anisotropy(16.0f);
			m_texture.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_texture.set_filter(graphics::TextureFilters::MAG_TRILINEAR);
			m_texture.set_mode(graphics::TextureModes::CLAMP_TO_BORDER);

			std::array<graphics::Vertex, 4> vertices;
			vertices[0].m_pos    = {0.0f, 0.0f};
			vertices[0].m_texels = {0.0f, 0.0f};

			vertices[1].m_pos    = {m_texture.get_width(), 0.0f};
			vertices[1].m_texels = {m_texture.get_width(), 0.0f};

			vertices[2].m_pos    = {m_texture.get_width(), m_texture.get_height()};
			vertices[2].m_texels = {m_texture.get_width(), m_texture.get_height()};

			vertices[3].m_pos    = {0.0f, m_texture.get_height()};
			vertices[3].m_texels = {0.0f, m_texture.get_height()};

			std::array<unsigned int, 6> indices = {0, 1, 3, 1, 2, 3};

			auto vbo = std::make_unique<graphics::VertexBuffer>();
			auto ibo = std::make_unique<graphics::IndexBuffer>();

			vbo->create(vertices, graphics::StorageFlag::STATIC_DRAW);
			ibo->create(indices, graphics::StorageFlag::STATIC_DRAW);

			m_vao.create(vbo, ibo);
		}

		void LoadingScreen::display(const int width, const int height, GLFWwindow* window)
		{
			m_shader.bind();
			m_shader.set_uniform<glm::mat4>("u_proj", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f));

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, width, height);
			glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glBindVertexArray(m_vao.id());
			glBindTexture(GL_TEXTURE_2D, m_texture.gl_texture());

			glDrawElements(GL_TRIANGLES, m_vao.index_count(), GL_UNSIGNED_INT, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
			glUseProgram(0);

			glfwSwapBuffers(window);
			m_displayed_at = std::chrono::high_resolution_clock::now();
		}

		void LoadingScreen::wait(const float milliseconds) noexcept
		{
			const auto time_to_wait = std::chrono::duration<float, std::milli>(milliseconds);
			const auto time_passed  = m_displayed_at - std::chrono::high_resolution_clock::now();

			if (time_passed < time_to_wait)
			{
				std::this_thread::sleep_for(time_to_wait - time_passed);
			}
		}
	} // namespace core
} // namespace galaxy