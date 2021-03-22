///
/// Label.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "Label.hpp"

namespace galaxy
{
	namespace ui
	{
		Label::Label() noexcept
		    : Widget {WidgetType::LABEL}
		{
		}

		Label::~Label() noexcept
		{
		}

		void Label::create(std::string_view text, std::string_view font)
		{
			m_text.load(font, m_theme->m_font_col);
			m_text.create(text);

			m_bounds.m_width  = m_text.get_width();
			m_bounds.m_height = m_text.get_height();

			m_theme->m_event_manager.subscribe<galaxy::events::MouseMoved>(*this);
		}

		void Label::update_text(std::string_view text)
		{
			m_text.create(text);

			m_bounds.m_width  = m_text.get_width();
			m_bounds.m_height = m_text.get_height();
		}

		void Label::on_event(const events::MouseMoved& mme) noexcept
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

		void Label::update(const double dt)
		{
		}

		void Label::render()
		{
			auto* shader = SL_HANDLE.shaderbook()->get("text");
			shader->bind();
			shader->set_uniform("u_cameraProj", m_theme->m_camera.get_proj());
			shader->set_uniform("u_cameraView", m_theme->m_camera.get_view());
			graphics::Renderer2D::draw_text(&m_text, &m_transform, shader);

			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		void Label::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_transform.set_pos(x, y);
		}

		nlohmann::json Label::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["x"]    = m_bounds.m_x;
			json["y"]    = m_bounds.m_y;
			json["text"] = m_text.get_text();
			json["font"] = m_text.get_font_id();

			json["tooltip"] = nlohmann::json::object();
			if (m_tooltip)
			{
				m_tooltip->serialize();
			}

			return json;
		}

		void Label::deserialize(const nlohmann::json& json)
		{
			m_tooltip = nullptr;
			m_transform.reset();

			create(json.at("text"), json.at("font"));
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