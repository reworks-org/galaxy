///
/// Loading.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <random>

#include <BS_thread_pool.hpp>
#include <glfw/glfw3.h>
#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "Loading.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	namespace core
	{
		constexpr const auto bg_vert = R"(
			#version 460 core

			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;

			out vec2 io_texels;

			uniform mat4 u_proj;
			uniform mat4 u_transform;

			void main()
			{
				gl_Position =  u_proj * u_transform * vec4(l_pos, 0.0, 1.0);
				io_texels = l_texels;
			}
		)";

		constexpr const auto bg_frag = R"(
			#version 460 core

			in vec2 io_texels;
			out vec4 io_frag_colour;

			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour = texture(u_texture, io_texels);
			}
		)";

		constexpr const auto text_vert = R"(
			#version 460 core
			layout(location = 0) in vec2 l_pos;
			layout(location = 1) in vec2 l_texels;

			out vec2 io_texels;

			uniform mat4 u_proj;
			uniform mat4 u_transform;

			void main()
			{
				io_texels = l_texels;
				gl_Position =  u_proj * u_transform * vec4(l_pos, 0.0, 1.0);
			}
		)";

		constexpr const auto text_frag = R"(
			#version 460 core

			in vec2 io_texels;
			out vec4 io_frag_colour;

			uniform vec4 u_colour;
			uniform sampler2D u_texture;

			void main()
			{
				io_frag_colour = texture(u_texture, io_texels) * u_colour;
			}
		)";

		Loading::LoadingText::LoadingText() noexcept
		{
		}

		Loading::LoadingText::~LoadingText() noexcept
		{
		}

		void Loading::LoadingText::create(const std::string& text, const float size, graphics::Font& font, const graphics::Colour& colour)
		{
			m_colour                = colour;
			const auto texture_size = font.get_text_size(text, size);

			m_texture.bind(true);

			std::size_t start = 0;
			std::size_t end   = text.find('\n');

			auto y_off = 0.0f;
			while (end != std::string::npos)
			{
				msdfgl_printf(0,
					y_off,
					font.handle(),
					size,
					0x000000FF,
					glm::value_ptr(m_texture.get_proj()),
					MSDFGL_KERNING,
					text.substr(start, end - start).c_str());

				y_off += font.vertical_advance(size);
				start = end + 1;
				end   = text.find('\n', start);
			}

			msdfgl_printf(0, y_off, font.handle(), size, 0x000000FF, glm::value_ptr(m_texture.get_proj()), MSDFGL_KERNING, text.substr(start, end).c_str());

			m_texture.unbind();

			auto vertices = graphics::Vertex::gen_quad_vertices(static_cast<int>(texture_size.x), static_cast<int>(texture_size.y));
			m_vao.create(vertices, graphics::StorageFlag::DYNAMIC_DRAW, graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);
		}

		Loading::Loading(std::string_view bg, const std::string& font)
		{
			m_bg_shader.load_raw(bg_vert, bg_frag);
			m_bg_shader.compile();

			m_text_shader.load_raw(text_vert, text_frag);
			m_text_shader.compile();

			auto& window = ServiceLocator<Window>::ref();
			m_bg_shader.set_uniform<glm::mat4>("u_proj", glm::ortho(0.0f, window.get_widthf(), window.get_heightf(), 0.0f, -1.0f, 1.0f));
			m_text_shader.set_uniform<glm::mat4>("u_proj", glm::ortho(0.0f, window.get_widthf(), window.get_heightf(), 0.0f, -1.0f, 1.0f));

			m_bg.load(bg);
			m_bg.set_anisotropy(16);
			m_bg.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_bg.set_filter(graphics::TextureFilters::MAG_TRILINEAR);
			m_bg.set_mode(graphics::TextureModes::REPEAT);

			m_font.load(font);
			m_font.build();

			auto array = graphics::Vertex::gen_quad_vertices(m_bg.get_width(), m_bg.get_height());
			m_vao.create(array, graphics::StorageFlag::STATIC_DRAW, graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, window.get_width(), window.get_height());
		}

		Loading::~Loading() noexcept
		{
		}

		void Loading::add_text(const std::string& text, const float size, const graphics::Colour& colour, glm::vec2 pos)
		{
			m_text_tf.set_pos(pos.x, pos.y);
			m_text = std::make_unique<Loading::LoadingText>();
			m_text->create(text, size, m_font, colour);
		}

		void Loading::add_tips(const std::string& lua_file, const float size, const graphics::Colour& colour, glm::vec2 pos)
		{
			m_tips_tf.set_pos(pos.x, pos.y);

			sol::state lua;
			lua.script_file(lua_file);

			std::vector<std::string>& tips = lua["tips"];

			for (const auto& tip : tips)
			{
				m_tips.emplace_back(std::make_unique<Loading::LoadingText>());
				m_tips.back()->create(tip, size, m_font, colour);
				m_tip_order.push_back(m_tips.size() - 1);
			}

			auto rd  = std::random_device {};
			auto rng = std::default_random_engine {rd()};
			std::shuffle(m_tip_order.begin(), m_tip_order.end(), rng);
		}

		void Loading::start(const std::function<void(void)>& lambda)
		{
			auto& tp        = ServiceLocator<BS::thread_pool>::ref();
			m_thread_handle = tp.submit(lambda);
		}

		void Loading::display()
		{
			auto& window = ServiceLocator<Window>::ref();

			while (!meta::is_work_done(m_thread_handle))
			{
				m_tf.translate(100.0f, 0.0f);

				glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glBindVertexArray(m_vao.id());
				glBindTexture(GL_TEXTURE_2D, m_bg.handle());
				m_bg_shader.bind();
				m_bg_shader.set_uniform("u_transform", m_tf.get_transform());
				glDrawElements(GL_TRIANGLES, m_vao.index_count(), GL_UNSIGNED_INT, nullptr);

				if (m_text != nullptr)
				{
					glBindVertexArray(m_text->m_vao.id());
					glBindTexture(GL_TEXTURE_2D, m_text->m_texture.get_texture());
					m_text_shader.bind();
					m_text_shader.set_uniform("u_transform", m_text_tf.get_transform());
					m_text_shader.set_uniform("u_colour", m_text->m_colour);
					glDrawElements(GL_TRIANGLES, m_text->m_vao.index_count(), GL_UNSIGNED_INT, nullptr);
				}

				for (const auto& idx : m_tip_order)
				{
					auto& tip = m_tips[idx];
					glBindVertexArray(tip->m_vao.id());
					glBindTexture(GL_TEXTURE_2D, tip->m_texture.get_texture());
					m_text_shader.bind();
					m_text_shader.set_uniform("u_transform", m_tips_tf.get_transform());
					m_text_shader.set_uniform("u_colour", tip->m_colour);
					glDrawElements(GL_TRIANGLES, tip->m_vao.index_count(), GL_UNSIGNED_INT, nullptr);
				}

				glfwSwapBuffers(window.handle());
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
			glUseProgram(0);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glfwSwapBuffers(window.handle());

			m_thread_handle.get();
		}
	} // namespace core
} // namespace galaxy