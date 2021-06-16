///
/// TextInput.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/resource/FontBook.hpp"
#include "galaxy/resource/ShaderBook.hpp"

#include "TextInput.hpp"

namespace galaxy
{
	namespace ui
	{
		TextInput::TextInput() noexcept
		    : Widget {WidgetType::TEXTINPUT}, m_total_chars {0}, m_draw_cursor {false}, m_border_width {0.0f}, m_text_input {nullptr}, m_is_focus {false}
		{
			m_timer.set_repeating(true);
		}

		TextInput::~TextInput() noexcept
		{
			m_text_input = nullptr;
		}

		void TextInput::create(std::string_view input_bg, std::string_view font, float border_width)
		{
			m_border_width = border_width;
			m_box.create(input_bg);

			m_bounds.m_width  = m_box.get_width();
			m_bounds.m_height = m_box.get_height();

			m_text.load(font, m_theme->m_font_col);
			m_text.create("");
			m_total_chars = (std::floor((m_bounds.m_width - (m_border_width * 2.0f)) / static_cast<float>(SL_HANDLE.fontbook()->get(font)->get_width("X")))) - 1;

			components::Primitive2D::PrimitiveData data;
			data.m_colour    = m_theme->m_font_col;
			data.m_start_end = std::make_optional<glm::vec4>(0.0f, 0.0f, 0.0f, m_bounds.m_height - (m_border_width * 4.0f));
			m_cursor.create<graphics::Primitives::LINE>(data);

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

			m_theme->m_sb.add(&m_box, &m_box_transform, 0);
			m_theme->m_event_manager.subscribe<events::MouseMoved>(*this);
			m_theme->m_event_manager.subscribe<events::MousePressed>(*this);
			m_theme->m_event_manager.subscribe<events::KeyDown>(*this);
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
			auto* text_shader = SL_HANDLE.shaderbook()->get("text");
			text_shader->bind();
			text_shader->set_uniform("u_cameraProj", m_theme->m_camera.get_proj());
			text_shader->set_uniform("u_cameraView", m_theme->m_camera.get_view());
			RENDERER_2D().draw_text(&m_text, &m_text_transform, text_shader);

			if (m_draw_cursor && m_is_focus)
			{
				auto* line_shader = SL_HANDLE.shaderbook()->get("line");
				line_shader->bind();
				line_shader->set_uniform("u_cameraProj", m_theme->m_camera.get_proj());
				line_shader->set_uniform("u_cameraView", m_theme->m_camera.get_view());
				RENDERER_2D().draw_line(&m_cursor, &m_cursor_transform, line_shader);
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
			m_box_transform.set_pos(x, y);
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

		nlohmann::json TextInput::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["input-bg"]     = m_box.get_tex_id();
			json["font"]         = m_text.get_font_id();
			json["border-width"] = m_border_width;
			json["x"]            = m_bounds.m_x;
			json["y"]            = m_bounds.m_y;

			json["tooltip"] = nlohmann::json::object();
			if (m_tooltip)
			{
				json["tooltip"] = json["tooltip"] = m_tooltip->serialize();
			}

			return json;
		}

		void TextInput::deserialize(const nlohmann::json& json)
		{
			stop();
			m_total_chars = 0;
			m_text_input  = nullptr;
			m_box_transform.reset();
			m_cursor_transform.reset();
			m_text_transform.reset();
			m_timer.stop();
			m_tooltip = nullptr;

			create(json.at("input-bg"), json.at("font"), json.at("border-width"));
			set_pos(json.at("x"), json.at("y"));

			const auto& tooltip_json = json.at("tooltip");
			if (!tooltip_json.empty())
			{
				auto* tooltip = create_tooltip();
				tooltip->deserialize(tooltip_json);
			}
		}
	} // namespace ui
} // namespace galaxy