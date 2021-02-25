///
/// TextInput.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/res/FontBook.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "TextInput.hpp"

namespace galaxy
{
	namespace ui
	{
		TextInput::TextInput() noexcept
		    : m_total_chars {0}, m_draw_cursor {false}, m_border_width {0.0f}, m_text_input {nullptr}, m_is_focus {false}
		{
			m_timer.set_repeating(true);
		}

		TextInput::~TextInput() noexcept
		{
			m_text_input = nullptr;
		}

		void TextInput::create(std::string_view textinput, std::string_view font, float border_width)
		{
			m_border_width = border_width;
			m_batched.create(textinput);

			m_bounds.m_width  = m_batched.get_width();
			m_bounds.m_height = m_batched.get_height();

			m_text.load(font, m_theme->m_font_col);
			m_text.create("");
			m_total_chars = (std::floor((m_bounds.m_width - (m_border_width * 2.0f)) / static_cast<float>(SL_HANDLE.fontbook()->get(font)->get_width("X")))) - 1;

			m_cursor.create(m_theme->m_font_col, 0.0f, 0.0f, 0.0f, m_bounds.m_height - (m_border_width * 4.0f));
			m_line_shader = SL_HANDLE.shaderbook()->get("line");
			m_text_shader = SL_HANDLE.shaderbook()->get("text");

			// clang-format off
			m_timer.set([&]()
			{
				if (m_is_focus)
				{
					m_draw_cursor = !m_draw_cursor;
				}
			}, 1000);
			m_timer.start();
			// clang-format on
		}

		void TextInput::on_event(const events::MouseMoved& mme) noexcept
		{
			if (m_tooltip)
			{
				if (m_bounds.contains(mme.m_x, mme.m_y))
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mme.m_x, mme.m_y);
				}
				else
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void TextInput::on_event(const events::MousePressed& mpe) noexcept
		{
			if (m_bounds.contains(mpe.m_x, mpe.m_y))
			{
				if (!m_is_focus)
				{
					m_text_input  = SL_HANDLE.window()->begin_text_input();
					*m_text_input = "";
					m_draw_cursor = true;
					m_is_focus    = true;
				}

				if (m_sound != nullptr)
				{
					m_sound->play();
				}

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
			else
			{
				SL_HANDLE.window()->end_text_input();
			}
		}

		void TextInput::on_event(const events::KeyDown& kde) noexcept
		{
			if (m_text_input != nullptr)
			{
				switch (kde.m_keycode)
				{
					case input::Keys::ENTER:
						stop();
						break;

					case input::Keys::BACKSPACE:
						if (m_text_input->length() > 0)
						{
							m_text_input->pop_back();
						}
						break;

					case input::Keys::DEL:
						*m_text_input = "";
						break;
				}
			}
		}

		void TextInput::update(const double dt)
		{
			if (m_is_focus)
			{
				m_timer.update(dt);
				if (m_text_input != nullptr)
				{
					while (m_text_input->length() > m_total_chars)
					{
						m_text_input->pop_back();
					}

					m_text.create(*m_text_input);

					if (m_draw_cursor)
					{
						float y_pos = m_bounds.m_y + (m_bounds.m_height / 2.0f);
						y_pos -= (m_bounds.m_height - (m_border_width * 4.0f)) / 2.0f;

						m_cursor_transform.set_pos(m_bounds.m_x + (m_border_width * 2.0f) + m_text.get_width(), y_pos);
					}
				}
			}
			else
			{
				stop();
			}
		}

		void TextInput::render()
		{
			m_text_shader->bind();
			m_text_shader->set_uniform("u_cameraProj", m_theme->m_projection);
			m_text_shader->set_uniform("u_cameraView", m_theme->m_transform.get_transform());
			graphics::Renderer::submit_text(&m_text, &m_text_transform, m_text_shader);

			if (m_draw_cursor && m_is_focus)
			{
				m_line_shader->bind();
				m_line_shader->set_uniform("u_cameraProj", m_theme->m_projection);
				m_line_shader->set_uniform("u_cameraView", m_theme->m_transform.get_transform());
				graphics::Renderer::submit_line(&m_cursor, &m_cursor_transform, m_line_shader);
			}

			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		void TextInput::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_transform.set_pos(x, y);
			m_text_transform.set_pos(m_bounds.m_x + (m_border_width * 2.0f), (m_bounds.m_y + (m_bounds.m_height / 2.0f)) - (m_text.get_height() / 2.0f));
		}

		void TextInput::stop() noexcept
		{
			if (m_text_input != nullptr)
			{
				SL_HANDLE.window()->end_text_input();
				m_draw_cursor = false;
				m_is_focus    = false;
			}
		}
	} // namespace ui
} // namespace galaxy