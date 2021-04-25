///
/// Image.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <chrono>

#include "Image.hpp"

namespace galaxy
{
	namespace ui
	{
		Image::Image() noexcept
		    : Widget {WidgetType::IMAGE}
		{
		}

		void Image::create(std::string_view name)
		{
			m_sprite.create(name);

			m_bounds.m_width  = m_sprite.get_width();
			m_bounds.m_height = m_sprite.get_height();

			m_theme->m_sb.add(&m_sprite, &m_transform, 0);
			m_theme->m_event_manager.subscribe<galaxy::events::MouseMoved>(*this);
		}

		void Image::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_transform.set_pos(x, y);
		}

		void Image::on_event(const events::MouseMoved& mme) noexcept
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

		void Image::update(const double dt)
		{
		}

		void Image::render()
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		nlohmann::json Image::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["x"]     = m_bounds.m_x;
			json["y"]     = m_bounds.m_y;
			json["image"] = m_sprite.get_tex_id();

			json["tooltip"] = nlohmann::json::object();
			if (m_tooltip)
			{
				json["tooltip"] = m_tooltip->serialize();
			}

			return json;
		}

		void Image::deserialize(const nlohmann::json& json)
		{
			m_tooltip = nullptr;
			m_transform.reset();

			create(json.at("image"));
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