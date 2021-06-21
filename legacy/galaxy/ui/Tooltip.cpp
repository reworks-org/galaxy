///
/// Tooltip.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/resource/ShaderBook.hpp"

#include "Tooltip.hpp"

#define ORTHO_FAR_24BIT 16777215

namespace galaxy
{
	namespace ui
	{
		Tooltip::Tooltip() noexcept
		    : Serializable {this}, m_draw {false}, m_theme {nullptr}
		{
			m_cursor_size = SL_HANDLE.window()->cursor_size();
		}

		Tooltip::Tooltip(Tooltip&& t) noexcept
		    : Serializable {this}
		{
			this->m_draw           = t.m_draw;
			this->m_text           = std::move(t.m_text);
			this->m_text_transform = std::move(t.m_text_transform);
			this->m_theme          = t.m_theme;
			this->m_cursor_size    = std::move(t.m_cursor_size);
		}

		Tooltip& Tooltip::operator=(Tooltip&& t) noexcept
		{
			if (this != &t)
			{
				this->m_draw           = t.m_draw;
				this->m_text           = std::move(t.m_text);
				this->m_text_transform = std::move(t.m_text_transform);
				this->m_theme          = t.m_theme;
				this->m_cursor_size    = std::move(t.m_cursor_size);
			}

			return *this;
		}

		Tooltip::~Tooltip() noexcept
		{
			m_theme = nullptr;
		}

		void Tooltip::create(std::string_view text, std::string_view font)
		{
			m_text.load(font, m_theme->m_font_col);
			m_text.create(text, ORTHO_FAR_24BIT);
		}

		void Tooltip::render()
		{
			RENDERER_2D().bind_text();
			RENDERER_2D().draw_text(&m_text, &m_text_transform);
		}

		void Tooltip::update_text(std::string_view text)
		{
			m_text.update(text);
		}

		void Tooltip::update_pos(double x, double y) noexcept
		{
			x += m_cursor_size.x;
			y += m_cursor_size.y;

			m_text_transform.set_pos(x, y);
		}

		void Tooltip::can_draw(const bool can_draw) noexcept
		{
			m_draw = can_draw;
		}

		const bool Tooltip::can_draw() const noexcept
		{
			return m_draw;
		}

		nlohmann::json Tooltip::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["can-draw"]       = m_draw;
			json["text"]           = m_text.serialize();
			json["text-transform"] = m_text_transform.serialize();

			return json;
		}

		void Tooltip::deserialize(const nlohmann::json& json)
		{
			m_draw = json.at("can-draw");
			m_text.deserialize(json.at("text"));
			m_text_transform.deserialize(json.at("text-transform"));
		}
	} // namespace ui
} // namespace galaxy