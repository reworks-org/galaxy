///
/// Textbox.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/resource/ShaderBook.hpp"

#include "Textbox.hpp"

#define SPEAKER_OFFSET 3

namespace galaxy
{
	namespace ui
	{
		Textbox::Message::Message() noexcept
		    : m_text {""}, m_speaker {""}
		{
		}

		Textbox::Message::Message(std::string_view text) noexcept
		    : m_text {text}, m_speaker {""}
		{
		}

		Textbox::Message::Message(std::string_view text, std::string_view speaker) noexcept
		    : m_text {text}, m_speaker {speaker}
		{
			m_speaker_spr.load(speaker);
			m_speaker_spr.create();
		}

		Textbox::Message::Message(Message&& m) noexcept
		{
			this->m_speaker     = std::move(m.m_speaker);
			this->m_speaker_spr = std::move(m.m_speaker_spr);
			this->m_speaker_tf  = std::move(m.m_speaker_tf);
			this->m_text        = std::move(m.m_text);
		}

		Textbox::Message& Textbox::Message::operator=(Message&& m) noexcept
		{
			if (this != &m)
			{
				this->m_speaker     = std::move(m.m_speaker);
				this->m_speaker_spr = std::move(m.m_speaker_spr);
				this->m_speaker_tf  = std::move(m.m_speaker_tf);
				this->m_text        = std::move(m.m_text);
			}

			return *this;
		}

		Textbox::Textbox() noexcept
		    : Widget {WidgetType::TEXTBOX}, m_border_width {0.0f}, m_messages_index {0}, m_char_index {0}, m_draw_lower {false}, m_prev_text {""}, m_ind_x {0.0f}, m_ind_y {0.0f}
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

			m_box.create(box);
			m_indicator.create(indicator, 0.0f);

			m_bounds.m_width  = m_box.get_width();
			m_bounds.m_height = m_box.get_height();

			m_text.load(font, m_theme->m_font_col);
			m_text.create("");
			m_text_transform.set_pos(m_bounds.m_x + m_border_width, m_bounds.m_y + m_border_width);

			// clang-format off
			m_indicator_timer.set([&]()
			{
				if (m_draw_lower)
				{
					m_draw_lower = false;
					m_indicator_transform.set_pos(m_ind_x, m_ind_y);
				}
				else
				{
					m_draw_lower = true;
					m_indicator_transform.set_pos(m_ind_x, m_ind_y + (m_indicator.get_height() / 4.0f));
				}
			},
			800);
			m_indicator_timer.start();

			m_draw_text_timer.set([&]()
			{
				if (!(m_messages_index >= m_messages.size()))
				{
					if (!(m_char_index >= m_messages[m_messages_index].m_text.size()))
					{
						m_char_index++;
					}
				}
			},
			150);
			m_draw_text_timer.start();
			// clang-format on

			m_theme->m_sb.add(&m_box, &m_box_transform, 0);
			m_theme->m_sb.add(&m_indicator, &m_indicator_transform, 1);
			m_theme->m_event_manager.subscribe<galaxy::events::MouseMoved>(*this);
			m_theme->m_event_manager.subscribe<galaxy::events::KeyDown>(*this);
		}

		void Textbox::on_event(const events::MouseMoved& mme) noexcept
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

		void Textbox::on_event(const events::KeyDown& kde) noexcept
		{
			if (kde.m_keycode == input::Keys::ENTER)
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
			m_draw_text_timer.update(dt);
			m_indicator_timer.update(dt);
			if (!(m_messages_index >= m_messages.size()))
			{
				if (!(m_char_index >= m_messages[m_messages_index].m_text.size()))
				{
					m_indicator.set_opacity(0.0f);

					auto cur_text = m_messages[m_messages_index].m_text.substr(0, m_char_index);
					if (cur_text != m_prev_text)
					{
						m_text.create(cur_text);
						m_prev_text = cur_text;
					}
				}
				else
				{
					m_indicator.set_opacity(1.0f);
				}
			}
		}

		void Textbox::render()
		{
			if (!(m_messages_index >= m_messages.size()))
			{
				auto& cur_msg = m_messages[m_messages_index];
				if (!cur_msg.m_speaker.empty())
				{
					auto* ss = SL_HANDLE.shaderbook()->get("sprite");
					ss->bind();
					ss->set_uniform("u_cameraProj", m_theme->m_camera.get_proj());
					ss->set_uniform("u_cameraView", m_theme->m_camera.get_view());
					RENDERER_2D().draw_sprite(&cur_msg.m_speaker_spr, &cur_msg.m_speaker_tf, ss);
				}
			}

			auto* ts = SL_HANDLE.shaderbook()->get("text");
			ts->bind();
			ts->set_uniform("u_cameraProj", m_theme->m_camera.get_proj());
			ts->set_uniform("u_cameraView", m_theme->m_camera.get_view());
			RENDERER_2D().draw_text(&m_text, &m_text_transform, ts);

			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		void Textbox::set_text(std::span<Textbox::Message> messages) noexcept
		{
			m_messages.clear();
			m_messages.insert(m_messages.end(), std::make_move_iterator(messages.begin()), std::make_move_iterator(messages.end()));

			for (auto& msg : m_messages)
			{
				msg.m_speaker_tf.set_pos((m_bounds.m_x + m_bounds.m_width) - msg.m_speaker_spr.get_width(), m_bounds.m_y - (msg.m_speaker_spr.get_height() + SPEAKER_OFFSET));
			}
		}

		void Textbox::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_box_transform.set_pos(x, y);

			m_text_transform.set_pos(m_bounds.m_x + m_border_width, m_bounds.m_y + m_border_width);

			m_ind_x = ((m_bounds.m_x + m_bounds.m_width) - m_indicator.get_width()) - m_border_width;
			m_ind_y = ((m_bounds.m_y + m_bounds.m_height) - m_indicator.get_height()) - m_border_width;
		}

		nlohmann::json Textbox::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["box"]          = m_box.get_tex_id();
			json["indicator"]    = m_indicator.get_tex_id();
			json["font"]         = m_text.get_font_id();
			json["border-width"] = m_border_width;
			json["x"]            = m_bounds.m_x;
			json["y"]            = m_bounds.m_y;

			nlohmann::ordered_json ojson = "[]"_json;
			for (const auto& message : m_messages)
			{
				const auto jstr = std::format("{text = {0}, speaker = {1}}", message.m_text, message.m_speaker);
				ojson.push_back(nlohmann::json::parse(jstr));
			}
			json["messages"] = ojson;

			json["tooltip"] = nlohmann::json::object();
			if (m_tooltip)
			{
				json["tooltip"] = json["tooltip"] = m_tooltip->serialize();
			}

			return json;
		}

		void Textbox::deserialize(const nlohmann::json& json)
		{
			m_indicator_timer.stop();
			m_draw_text_timer.stop();
			m_draw_lower = false;
			m_messages.clear();
			m_messages_index = 0;
			m_char_index     = 0;
			m_prev_text      = "";
			m_box_transform.reset();
			m_text_transform.reset();
			m_indicator_transform.reset();
			m_tooltip = nullptr;

			create(json.at("box"), json.at("indicator"), json.at("font"), json.at("border-width"));
			set_pos(json.at("x"), json.at("y"));

			const auto& messages = json.at("messages");
			for (const auto& message : messages)
			{
				m_messages.emplace_back(messages.at("text").get<std::string>(), messages.at("speaker").get<std::string>());
			}

			for (auto& msg : m_messages)
			{
				msg.m_speaker_tf.set_pos((m_bounds.m_x + m_bounds.m_width) - msg.m_speaker_spr.get_width(), m_bounds.m_y - (msg.m_speaker_spr.get_height() + SPEAKER_OFFSET));
			}

			const auto& tooltip_json = json.at("tooltip");
			if (!tooltip_json.empty())
			{
				auto* tooltip = create_tooltip();
				tooltip->deserialize(tooltip_json);
			}
		}
	} // namespace ui
} // namespace galaxy