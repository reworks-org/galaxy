///
/// TextInput.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/graphics/Window.hpp"

#include "TextInput.hpp"

namespace galaxy
{
	namespace ui
	{
		TextInput::TextInput()
		    : m_total_chars {0}, m_draw_cursor {false}, m_border_width {0.0f}, m_text_input {nullptr}, m_is_focus {false}
		{
			m_timer.set_repeating(true);
		}

		TextInput::~TextInput()
		{
			m_text_input = nullptr;
		}

		void TextInput::create(std::string_view textinput, std::string_view font, float border_width)
		{
			m_border_width = border_width;

			auto opt = m_theme->m_atlas.get_region(textinput);
			if (opt != std::nullopt)
			{
				create(opt.value(), 0);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Unable to create image widget from {0}.", textinput);
			}

			m_bounds.m_width  = get_width();
			m_bounds.m_height = get_height();

			auto* fontptr = m_theme->m_fonts->get(font);
			m_text.load(fontptr, m_theme->m_font_col);
			m_text.create("");
			m_total_chars = (std::floor((m_bounds.m_width - (m_border_width * 2.0f)) / static_cast<float>(fontptr->get_width("X")))) - 1;

			m_cursor.create(m_theme->m_font_col, 0.0f, 0.0f, 0.0f, m_bounds.m_height - (m_border_width * 4.0f));
			m_timer.launch([&]() {
				if (m_is_focus)
				{
					m_draw_cursor = !m_draw_cursor;
				}
			},
				       1000);
		}

		void TextInput::on_event(const events::MouseMoved& mme)
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

		void TextInput::on_event(const events::MousePressed& mpe)
		{
			if (m_bounds.contains(mpe.m_x, mpe.m_y))
			{
				if (!m_is_focus)
				{
					m_text_input  = m_theme->m_window->begin_text_input();
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
				m_theme->m_window->end_text_input();
			}
		}

		void TextInput::on_event(const events::KeyDown& kde)
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
				if (m_text_input != nullptr)
				{
					while (m_text_input->length() > m_total_chars)
					{
						m_text_input->pop_back();
					}

					m_text.update_text(*m_text_input);

					if (m_draw_cursor)
					{
						float y_pos = m_bounds.m_y + (m_bounds.m_height / 2.0f);
						y_pos -= (m_bounds.m_height - (m_border_width * 4.0f)) / 2.0f;

						m_cursor.set_pos(m_bounds.m_x + (m_border_width * 2.0f) + m_text.get_width(), y_pos);
					}
				}
			}
			else
			{
				stop();
			}
		}

		void TextInput::render(graphics::Camera& camera)
		{
			auto ss = m_theme->m_shaders->get("text");

			ss->bind();
			ss->set_uniform("u_cameraProj", camera.get_proj());
			ss->set_uniform("u_cameraView", camera.get_transform());
			m_theme->m_renderer->draw_text(m_text, *ss);

			if (m_draw_cursor && m_is_focus)
			{
				auto ls = m_theme->m_shaders->get("line");

				ls->bind();
				ls->set_uniform("u_cameraProj", camera.get_proj());
				ls->set_uniform("u_cameraView", camera.get_transform());
				m_theme->m_renderer->draw_line(m_cursor, *ls);
			}

			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render(camera);
				}
			}
		}

		void TextInput::set_pos(const float x, const float y)
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_pos.x      = x;
			m_pos.y      = y;

			m_translation = glm::translate(glm::mat4 {1.0f}, {x, y, 0.0f});
			m_dirty       = true;

			m_text.set_pos(m_bounds.m_x + (m_border_width * 2.0f), (m_bounds.m_y + (m_bounds.m_height / 2.0f)) - (m_text.get_height() / 2.0f));
		}

		void TextInput::stop()
		{
			if (m_text_input != nullptr)
			{
				m_theme->m_window->end_text_input();
				m_draw_cursor = false;
				m_is_focus    = false;
			}
		}
	} // namespace ui
} // namespace galaxy