///
/// Button.cpp
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

#include "Button.hpp"

namespace galaxy
{
	namespace ui
	{
		Button::Button() noexcept
		    : Widget {WidgetType::BUTTON}, m_state {Button::State::DEFAULT}
		{
		}

		void Button::create(std::string_view def, std::string_view pressed, std::string_view hover)
		{
			m_sprite.create(def);
			m_regions[0] = def;
			m_regions[1] = pressed;
			m_regions[2] = hover;

			m_bounds.m_width  = m_sprite.get_width();
			m_bounds.m_height = m_sprite.get_height();

			m_theme->m_sb.add(&m_sprite, &m_transform, 0);
			m_theme->m_event_manager.subscribe<galaxy::events::MouseMoved>(*this);
			m_theme->m_event_manager.subscribe<galaxy::events::MousePressed>(*this);
			m_theme->m_event_manager.subscribe<galaxy::events::MouseReleased>(*this);
		}

		void Button::set_onclick(std::string_view script_id)
		{
			m_onclick = static_cast<std::string>(script_id);
		}

		void Button::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_transform.set_pos(x, y);
		}

		void Button::on_event(const events::MouseMoved& mme) noexcept
		{
			if (m_bounds.contains(mme.m_x, mme.m_y))
			{
				m_state = Button::State::HOVER;

				if (m_tooltip)
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mme.m_x, mme.m_y);
				}
			}
			else
			{
				m_state = Button::State::DEFAULT;

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void Button::on_event(const events::MousePressed& mpe) noexcept
		{
			if (m_bounds.contains(mpe.m_x, mpe.m_y))
			{
				m_state = Button::State::PRESSED;

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

		void Button::on_event(const events::MouseReleased& mre) noexcept
		{
			if (m_bounds.contains(mre.m_x, mre.m_y))
			{
				m_state = Button::State::HOVER;

				if (m_tooltip)
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mre.m_x, mre.m_y);
				}
			}
			else
			{
				m_state = Button::State::DEFAULT;

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void Button::update(const double dt)
		{
			switch (m_state)
			{
				case Button::State::DEFAULT:
					m_sprite.set_region(m_regions[0]);
					break;

				case Button::State::PRESSED:
					m_sprite.set_region(m_regions[1]);
					break;

				case Button::State::HOVER:
					m_sprite.set_region(m_regions[2]);
					break;
			}
		}

		void Button::render()
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		nlohmann::json Button::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["x"]               = m_bounds.m_x;
			json["y"]               = m_bounds.m_y;
			json["state"]           = static_cast<std::string>(magic_enum::enum_name<Button::State>(m_state));
			json["on-click-script"] = m_onclick;
			json["default"]         = m_regions[0];
			json["pressed"]         = m_regions[1];
			json["hover"]           = m_regions[2];

			json["tooltip"] = nlohmann::json::object();
			if (m_tooltip)
			{
				json["tooltip"] = m_tooltip->serialize();
			}

			return json;
		}

		void Button::deserialize(const nlohmann::json& json)
		{
			m_tooltip = nullptr;
			m_transform.reset();

			create(json.at("default"), json.at("pressed"), json.at("hover"));
			set_pos(json.at("x"), json.at("y"));

			m_state   = magic_enum::enum_cast<Button::State>(json.at("state").get<std::string>()).value();
			m_onclick = json.at("on-click-script");

			const auto& tooltip_json = json.at("tooltip");
			if (!tooltip_json.empty())
			{
				auto* tooltip = create_tooltip();
				tooltip->deserialize(tooltip_json);
			}
		}
	} // namespace ui
} // namespace galaxy