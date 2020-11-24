///
/// Textbox.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Renderer.hpp>

#include "Textbox.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Widget specific namespace.
	///
	namespace widget
	{
		Textbox::Textbox()
		    : m_text_to_draw {""}, m_border_width {0.0f}, m_messages_index {0}, m_char_index {0}, m_draw_lower {false}, m_prev_text {""}, m_ind_x {0.0f}, m_ind_y {0.0f}
		{
			m_indicator_timer.set_repeating(true);
			m_draw_text_timer.set_repeating(true);
		}

		Textbox::~Textbox()
		{
			m_messages.clear();
			m_indicator_timer.stop();
			m_draw_text_timer.stop();
		}

		void Textbox::create(std::string_view box, std::string_view indicator, std::string_view font, float border_width)
		{
			m_border_width = border_width;

			auto box_opt = m_theme->m_atlas.get_region(box);
			auto ind_opt = m_theme->m_atlas.get_region(indicator);

			if (box_opt != std::nullopt)
			{
				create(box_opt.value(), 1);
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create textbox widget from {0}.", box);
			}

			m_bounds.m_width  = get_width();
			m_bounds.m_height = get_height();

			if (ind_opt != std::nullopt)
			{
				m_indicator.create(ind_opt.value(), 3);
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create arrow indicator from {0}.", indicator);
			}

			m_indicator.set_opacity(0.0f);

			m_text.load(m_theme->m_fonts->get(font), m_theme->m_font_col);
			m_text.create("");
			m_text.set_pos(m_bounds.m_x + m_border_width, m_bounds.m_y + m_border_width);

			// clang-format off
			m_indicator_timer.launch([&]()
			{
				std::lock_guard<std::mutex> lock{ m_mutex };
				
				if (m_draw_lower)
				{
					m_draw_lower = false;
					m_indicator.set_pos(m_ind_x, m_ind_y);
				}
				else
				{
					m_draw_lower = true;
					m_indicator.set_pos(m_ind_x, m_ind_y + (m_indicator.get_height() / 4.0f));
				}
			},
			800);
			
			m_draw_text_timer.launch([&]()
			{
				if (!(m_messages_index >= m_messages.size()))
				{
					if (!(m_char_index >= m_messages[m_messages_index].size()))
					{
						m_char_index++;
					}
				}
			},
			150);
			// clang-format on
		}

		void Textbox::on_event(const pr::MouseMovedEvent& mme)
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

		void Textbox::on_event(const pr::KeyDownEvent& kde)
		{
			if (kde.m_keycode == pr::Keys::ENTER)
			{
				m_messages_index++;
				if (m_messages_index > (m_messages.size() - 1))
				{
					m_messages_index = (m_messages.size() - 1);
				}

				m_char_index = 0;
			}
		}

		void Textbox::update(const double dt)
		{
			if (!(m_messages_index >= m_messages.size()))
			{
				if (!(m_char_index >= m_messages[m_messages_index].size()))
				{
					m_indicator.set_opacity(0.0f);

					auto cur_text = m_messages[m_messages_index].substr(0, m_char_index);
					if (cur_text != m_prev_text)
					{
						m_text.update_text(cur_text);
						m_prev_text = cur_text;
					}
				}
				else
				{
					m_indicator.set_opacity(1.0f);
				}
			}
		}

		void Textbox::render(qs::Camera& camera)
		{
			auto ts = m_theme->m_shaders->get("text");

			ts->bind();
			ts->set_uniform("u_cameraProj", camera.get_proj());
			ts->set_uniform("u_cameraView", camera.get_transform());
			m_theme->m_renderer->draw_text(m_text, *ts);

			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render(camera);
				}
			}
		}

		void Textbox::set_text(std::span<std::string> messages)
		{
			m_messages.clear();
			m_messages.reserve(messages.size());
			m_messages.assign(messages.begin(), messages.end());
		}

		void Textbox::set_pos(const float x, const float y)
		{
			std::lock_guard<std::mutex> lock {m_mutex};

			m_bounds.m_x = x;
			m_bounds.m_y = y;

			m_translation = glm::translate(glm::mat4 {1.0f}, {x, y, 0.0f});
			m_dirty       = true;

			m_text.set_pos(m_bounds.m_x + m_border_width, m_bounds.m_y + m_border_width);

			m_ind_x = ((m_bounds.m_x + m_bounds.m_width) - m_indicator.get_width()) - m_border_width;
			m_ind_y = ((m_bounds.m_y + m_bounds.m_height) - m_indicator.get_height()) - m_border_width;
		}
	} // namespace widget
} // namespace galaxy