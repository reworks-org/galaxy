///
/// ToggleButton.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <optional>

#include <magic_enum.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/res/ScriptBook.hpp"
#include "galaxy/res/TextureAtlas.hpp"

#include "ToggleButton.hpp"

namespace galaxy
{
	namespace ui
	{
		ToggleButton::ToggleButton() noexcept
		    : Widget {WidgetType::TOGGLEBUTTON}, m_state {ToggleButton::State::OFF}, m_on {false}
		{
		}

		void ToggleButton::create(std::string_view on, std::string_view off, std::string_view on_hover, std::string_view off_hover)
		{
			m_sprite.create(on);
			m_regions[0] = on;
			m_regions[1] = off;
			m_regions[2] = on_hover;
			m_regions[3] = off_hover;

			m_bounds.m_width  = m_sprite.get_width();
			m_bounds.m_height = m_sprite.get_height();

			m_theme->m_sb.add(&m_sprite, &m_transform, 0);
			m_theme->m_event_manager.subscribe<galaxy::events::MouseMoved>(*this);
			m_theme->m_event_manager.subscribe<galaxy::events::MousePressed>(*this);
		}

		void ToggleButton::set_onclick(std::string_view script_id)
		{
			m_onclick = static_cast<std::string>(script_id);
		}

		void ToggleButton::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;

			m_transform.set_pos(x, y);
		}

		void ToggleButton::on_event(const events::MouseMoved& mme) noexcept
		{
			if (m_bounds.contains(mme.m_x, mme.m_y))
			{
				if (m_on)
				{
					m_state = ToggleButton::State::ON_HOVER;
				}
				else
				{
					m_state = ToggleButton::State::OFF_HOVER;
				}

				if (m_tooltip)
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mme.m_x, mme.m_y);
				}
			}
			else
			{
				if (m_on)
				{
					m_state = ToggleButton::State::ON;
				}
				else
				{
					m_state = ToggleButton::State::OFF;
				}

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void ToggleButton::on_event(const events::MousePressed& mpe) noexcept
		{
			if (m_bounds.contains(mpe.m_x, mpe.m_y))
			{
				m_on = !m_on;
				if (m_on)
				{
					m_state = ToggleButton::State::ON_HOVER;
				}
				else
				{
					m_state = ToggleButton::State::OFF_HOVER;
				}

				if (!m_onclick.empty())
				{
					SL_HANDLE.scriptbook()->run(m_onclick);
				}

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void ToggleButton::update(const double dt)
		{
			switch (m_state)
			{
				case ToggleButton::State::ON:
					m_sprite.set_region(m_regions[0]);
					break;

				case ToggleButton::State::OFF:
					m_sprite.set_region(m_regions[1]);
					break;

				case ToggleButton::State::ON_HOVER:
					m_sprite.set_region(m_regions[2]);
					break;

				case ToggleButton::State::OFF_HOVER:
					m_sprite.set_region(m_regions[3]);
					break;
			}
		}

		void ToggleButton::render()
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		const bool ToggleButton::is_on() const noexcept
		{
			return m_on;
		}

		nlohmann::json ToggleButton::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["x"]               = m_bounds.m_x;
			json["y"]               = m_bounds.m_y;
			json["selected"]        = m_on;
			json["state"]           = static_cast<std::string>(magic_enum::enum_name<ToggleButton::State>(m_state));
			json["on-click-script"] = m_onclick;
			json["on"]              = m_regions[0];
			json["off"]             = m_regions[1];
			json["on_hover"]        = m_regions[2];
			json["off_hover"]       = m_regions[3];

			json["tooltip"] = nlohmann::json::object();
			if (m_tooltip)
			{
				m_tooltip->serialize();
			}

			return json;
		}

		void ToggleButton::deserialize(const nlohmann::json& json)
		{
			m_tooltip = nullptr;
			m_transform.reset();

			m_on      = json.at("selected");
			m_state   = magic_enum::enum_cast<ToggleButton::State>(json.at("state").get<std::string>()).value();
			m_onclick = json.at("on-click-script");

			create(json.at("on"), json.at("off"), json.at("on_hover"), json.at("off_hover"));
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