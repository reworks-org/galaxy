///
/// Slider.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "Slider.hpp"

#define ORTHO_FAR_24BIT 16777215

namespace galaxy
{
	namespace ui
	{
		Slider::Slider() noexcept
		    : Widget {WidgetType::SLIDER}, m_value {1.0f}, m_pressed {false}, m_marker_pos {0.0f, 0.0f, 0.0f, 0.0f}
		{
		}

		void Slider::create(std::string_view slider, std::string_view marker)
		{
			m_slider.create(slider, ORTHO_FAR_24BIT);
			m_marker.create(marker, ORTHO_FAR_24BIT);

			m_bounds.m_width  = m_slider.get_region().m_width;
			m_bounds.m_height = m_slider.get_region().m_height;

			m_theme->m_sb.add(&m_slider, &m_slider_transform);
			m_theme->m_sb.add(&m_marker, &m_marker_transform);
			m_theme->m_event_manager.subscribe<galaxy::events::MouseMoved>(*this);
			m_theme->m_event_manager.subscribe<galaxy::events::MousePressed>(*this);
			m_theme->m_event_manager.subscribe<galaxy::events::MouseReleased>(*this);
		}

		void Slider::on_event(const events::MouseMoved& mme) noexcept
		{
			if (m_bounds.contains(mme.m_x, mme.m_y))
			{
				if (m_pressed)
				{
					m_value = std::clamp((static_cast<float>(mme.m_x) - m_bounds.m_x) / m_bounds.m_width, 0.0f, 1.0f);
				}

				if (m_tooltip)
				{
					if (m_pressed)
					{
						m_tooltip->can_draw(false);
					}
					else
					{
						m_tooltip->can_draw(true);
						m_tooltip->update_pos(mme.m_x, mme.m_y);
					}
				}
			}
		}

		void Slider::on_event(const events::MousePressed& mpe) noexcept
		{
			if (m_bounds.contains(mpe.m_x, mpe.m_y))
			{
				m_pressed = true;
				m_value   = std::clamp((mpe.m_x - m_bounds.m_x) / m_bounds.m_width, 0.0f, 1.0f);
			}
		}

		void Slider::on_event(const events::MouseReleased& mre) noexcept
		{
			m_pressed = false;

			if (m_tooltip)
			{
				if (m_bounds.contains(mre.m_x, mre.m_y))
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mre.m_x, mre.m_y);
				}
			}
		}

		void Slider::update(const double dt)
		{
			m_marker_transform.set_pos((m_bounds.m_x + (m_bounds.m_width * m_value)) - (m_marker.get_region().m_width / 2.0f), m_bounds.m_y - (m_marker.get_region().m_height / 2.0f) + (m_bounds.m_height / 2.0f));
		}

		void Slider::render()
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		void Slider::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_slider_transform.set_pos(x, y);
		}

		const float Slider::value() const noexcept
		{
			return m_value;
		}

		const float Slider::percentage() const noexcept
		{
			return m_value * 100.0f;
		}

		nlohmann::json Slider::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["x"]       = m_bounds.m_x;
			json["y"]       = m_bounds.m_y;
			json["value"]   = m_value;
			json["pressed"] = m_pressed;
			json["slider"]  = m_slider.get_key();
			json["marker"]  = m_marker.get_key();

			json["tooltip"] = nlohmann::json::object();
			if (m_tooltip)
			{
				json["tooltip"] = m_tooltip->serialize();
			}

			return json;
		}

		void Slider::deserialize(const nlohmann::json& json)
		{
			m_tooltip = nullptr;
			m_slider_transform.reset();
			m_marker_transform.reset();

			create(json.at("slider"), json.at("marker"));
			set_pos(json.at("x"), json.at("y"));

			m_value   = json.at("value");
			m_pressed = json.at("pressed");

			const auto& tooltip_json = json.at("tooltip");
			if (!tooltip_json.empty())
			{
				auto* tooltip = create_tooltip();
				tooltip->deserialize(tooltip_json);
			}
		}
	} // namespace ui
} // namespace galaxy