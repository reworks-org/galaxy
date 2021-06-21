///
/// Progressbar.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "Progressbar.hpp"

namespace galaxy
{
	namespace ui
	{
		Progressbar::Progressbar() noexcept
		    : Widget {WidgetType::PROGRESSBAR}, m_progress {1.0f}, m_prev_progress {-1.0f}, m_bar_pos {0.0f, 0.0f, 0.0f, 0.0f}
		{
		}

		void Progressbar::create(std::string_view container, std::string_view bar)
		{
			m_container.create(container);
			m_bar.create(bar);

			m_bounds.m_width  = m_container.get_width();
			m_bounds.m_height = m_container.get_height();

			m_theme->m_sb.add(&m_container, &m_container_transform, 0);
			m_theme->m_sb.add(&m_bar, &m_bar_transform, 1);
			m_theme->m_event_manager.subscribe<galaxy::events::MouseMoved>(*this);
		}

		void Progressbar::on_event(const events::MouseMoved& mme) noexcept
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

		void Progressbar::update(const double dt)
		{
			if (m_progress != m_prev_progress)
			{
				m_bar.set_custom_width(m_bounds.m_width * m_progress);
				m_prev_progress = m_progress;
			}
		}

		void Progressbar::render()
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		void Progressbar::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;

			m_container_transform.set_pos(x, y);
			m_bar_transform.set_pos(x, y);
		}

		void Progressbar::set_progress(const float progress) noexcept
		{
			m_progress = std::clamp(progress, 0.0f, 1.0f);
		}

		const float Progressbar::progress() const noexcept
		{
			return m_progress;
		}

		const float Progressbar::percentage() const noexcept
		{
			return m_progress * 100.0f;
		}

		nlohmann::json Progressbar::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["x"]         = m_bounds.m_x;
			json["y"]         = m_bounds.m_y;
			json["container"] = m_container.get_tex_id();
			json["bar"]       = m_bar.get_tex_id();

			json["tooltip"] = nlohmann::json::object();
			if (m_tooltip)
			{
				json["tooltip"] = m_tooltip->serialize();
			}

			return json;
		}

		void Progressbar::deserialize(const nlohmann::json& json)
		{
			m_tooltip       = nullptr;
			m_progress      = 1.0f;
			m_prev_progress = -1.0f;
			m_bar_pos       = {0.0f, 0.0f, 0.0f, 0.0f};
			m_container_transform.reset();
			m_bar_transform.reset();

			create(json.at("container"), json.at("bar"));
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