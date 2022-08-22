///
/// Loading.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>
#include <glfw/glfw3.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "Loading.hpp"

using namespace std::chrono_literals;

///
/// Logo vertex shader.
///
constexpr const char* const logo_vert = R"(
	#version 460 core

	layout(location = 0) in vec2 l_pos;
	layout(location = 1) in vec2 l_texels;
	layout(location = 2) in vec4 l_colour;

	out vec2 io_texels;

	uniform mat4 u_proj;
	uniform mat4 u_transform;

	void main()
	{
		gl_Position =  u_proj * u_transform * vec4(l_pos, 0.0, 1.0);
		io_texels = l_texels;
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
		Loading::Loading() noexcept
		{
			m_shader.load_raw(logo_vert, logo_frag);
		}

		Loading::~Loading() noexcept
		{
		}

		void Loading::prep_window_for_loading() noexcept
		{
			auto& window = ServiceLocator<Window>::ref();

			m_shader.set_uniform<glm::mat4>("u_proj", glm::ortho(0.0f, window.get_widthf(), window.get_heightf(), 0.0f, -1.0f, 1.0f));

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, window.get_width(), window.get_height());

			auto vbo = std::make_unique<graphics::VertexBuffer>();
			auto ibo = std::make_unique<graphics::IndexBuffer>();

			auto array = graphics::Vertex::gen_quad_vertices(1, 1);
			vbo->create(array, graphics::StorageFlag::STATIC_DRAW);
			ibo->const_create(graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);

			m_vao.create(vbo, ibo);
		}

		void Loading::display_loadingscreen(std::string_view background) noexcept
		{
			if (!background.empty())
			{
				graphics::Texture bg;
				if (bg.load(background))
				{
					bg.set_anisotropy(16);
					bg.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
					bg.set_filter(graphics::TextureFilters::MAG_TRILINEAR);
					bg.set_mode(graphics::TextureModes::CLAMP_TO_BORDER);

					auto array = graphics::Vertex::gen_quad_vertices(bg.get_width(), bg.get_height());
					m_vao.sub_buffer(0, array);

					m_shader.bind();

					glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					glBindVertexArray(m_vao.id());
					glBindTexture(GL_TEXTURE_2D, bg.gl_texture());

					m_shader.set_uniform("u_transform", m_default.get_transform());
					glDrawElements(GL_TRIANGLES, m_vao.index_count(), GL_UNSIGNED_INT, nullptr);

					glfwSwapBuffers(ServiceLocator<Window>::ref().handle());

					glBindTexture(GL_TEXTURE_2D, 0);
					glBindVertexArray(0);
					m_shader.unbind();
				}
			}
		}

		void Loading::start_offthread_loading(const std::function<void(void)>& lambda)
		{
			auto& tp         = ServiceLocator<BS::thread_pool>::ref();
			m_loading_thread = std::move(tp.submit(lambda));
		}

		void Loading::display_bg_until_finished(std::string_view background, std::string_view logo) noexcept
		{
			if (!background.empty() && !logo.empty())
			{
				if (m_bg.load(background) && m_animated.load(logo))
				{
					m_bg.set_anisotropy(16);
					m_bg.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
					m_bg.set_filter(graphics::TextureFilters::MAG_TRILINEAR);
					m_bg.set_mode(graphics::TextureModes::CLAMP_TO_BORDER);

					auto array = graphics::Vertex::gen_quad_vertices(m_bg.get_width(), m_bg.get_height());
					m_vao.sub_buffer(0, array);

					m_animated.set_anisotropy(16);
					m_animated.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
					m_animated.set_filter(graphics::TextureFilters::MAG_TRILINEAR);
					m_animated.set_mode(graphics::TextureModes::CLAMP_TO_BORDER);

					auto vbo = std::make_unique<graphics::VertexBuffer>();
					auto ibo = std::make_unique<graphics::IndexBuffer>();

					auto array2 = graphics::Vertex::gen_quad_vertices(m_animated.get_width(), m_animated.get_height());
					vbo->create(array2, graphics::StorageFlag::STATIC_DRAW);
					ibo->const_create(graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);

					m_anim_vao.create(vbo, ibo);

					auto& window = ServiceLocator<Window>::ref();
					m_transform.set_pos(window.get_widthf() - m_animated.get_widthf(), window.get_heightf() - m_animated.get_heightf());

					m_shader.bind();
					m_transform.set_origin(m_animated.get_widthf() / 2.0f, m_animated.get_heightf() / 2.0f);

					while (!meta::is_work_done(m_loading_thread))
					{
						m_transform.rotate(8.0f);

						glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

						glBindVertexArray(m_vao.id());
						glBindTexture(GL_TEXTURE_2D, m_bg.gl_texture());
						m_shader.set_uniform("u_transform", m_default.get_transform());
						glDrawElements(GL_TRIANGLES, m_vao.index_count(), GL_UNSIGNED_INT, nullptr);

						glBindVertexArray(m_anim_vao.id());
						glBindTexture(GL_TEXTURE_2D, m_animated.gl_texture());
						m_shader.set_uniform("u_transform", m_transform.get_transform());
						glDrawElements(GL_TRIANGLES, m_anim_vao.index_count(), GL_UNSIGNED_INT, nullptr);

						glfwSwapBuffers(window.handle());
					}

					glBindTexture(GL_TEXTURE_2D, 0);
					glBindVertexArray(0);
					m_shader.unbind();
				}
			}

			m_loading_thread.get();
		}
	} // namespace core
} // namespace galaxy