///
/// Tooltip.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "Tooltip.hpp"

namespace galaxy
{
	namespace ui
	{
		Tooltip::Tooltip() noexcept
		    : m_draw {false}, m_border {0.0f}, m_theme {nullptr}, m_sprite_shader {nullptr}, m_text_shader {nullptr}
		{
			m_cursor_size = SL_HANDLE.window()->cursor_size();
		}

		Tooltip::Tooltip(Tooltip&& t) noexcept
		{
			this->m_draw             = t.m_draw;
			this->m_border           = t.m_border;
			this->m_sprite           = std::move(t.m_sprite);
			this->m_text             = std::move(t.m_text);
			this->m_sprite_transform = std::move(t.m_sprite_transform);
			this->m_text_transform   = std::move(t.m_text_transform);
			this->m_theme            = t.m_theme;
			this->m_sprite_shader    = t.m_sprite_shader;
			this->m_text_shader      = t.m_text_shader;
			this->m_cursor_size      = std::move(t.m_cursor_size);
		}

		Tooltip& Tooltip::operator=(Tooltip&& t) noexcept
		{
			if (this != &t)
			{
				this->m_draw             = t.m_draw;
				this->m_border           = t.m_border;
				this->m_sprite           = std::move(t.m_sprite);
				this->m_text             = std::move(t.m_text);
				this->m_sprite_transform = std::move(t.m_sprite_transform);
				this->m_text_transform   = std::move(t.m_text_transform);
				this->m_theme            = t.m_theme;
				this->m_sprite_shader    = t.m_sprite_shader;
				this->m_text_shader      = t.m_text_shader;
				this->m_cursor_size      = std::move(t.m_cursor_size);
			}

			return *this;
		}

		Tooltip::~Tooltip() noexcept
		{
			m_theme         = nullptr;
			m_sprite_shader = nullptr;
			m_text_shader   = nullptr;
		}

		void Tooltip::create(const TooltipTexture& tex, std::string_view text, std::string_view font)
		{
			// Multiply by 2 because there are two sides to a rectangle (on each axis).
			m_border = tex.m_border * 2.0f;

			m_text.load(font, m_theme->m_font_col);
			m_text.create(text);

			m_sprite.load(tex.m_texture);
			m_sprite.stretch(m_text.get_width() + m_border * 2, m_text.get_height() + m_border * 2);
			m_sprite.create();

			m_sprite_transform.set_rotation_origin(m_sprite.get_width() * 0.5f, m_sprite.get_height() * 0.5f);
			m_text_transform.set_rotation_origin(m_text.get_width() * 0.5f, m_text.get_height() * 0.5f);

			m_sprite_shader = SL_HANDLE.shaderbook()->get("sprite");
			m_text_shader   = SL_HANDLE.shaderbook()->get("text");
		}

		void Tooltip::render()
		{
			m_sprite_shader->bind();
			m_sprite_shader->set_uniform("u_cameraProj", m_theme->m_projection);
			m_sprite_shader->set_uniform("u_cameraView", m_theme->m_transform.get_transform());
			graphics::Renderer::submit_sprite(&m_sprite, &m_sprite_transform, m_sprite_shader);

			m_text_shader->bind();
			m_text_shader->set_uniform("u_cameraProj", m_theme->m_projection);
			m_text_shader->set_uniform("u_cameraView", m_theme->m_transform.get_transform());
			graphics::Renderer::submit_text(&m_text, &m_text_transform, m_text_shader);
		}

		void Tooltip::update_text(std::string_view text)
		{
			m_text.create(text);
			m_sprite.stretch(m_text.get_width() + m_border, m_text.get_height() + m_border);
			m_sprite.create();
		}

		void Tooltip::update_pos(double x, double y) noexcept
		{
			x += m_cursor_size.x;
			y += m_cursor_size.y;

			m_sprite_transform.set_pos(x, y);
			m_text_transform.set_pos(x + m_border, y + ((m_sprite.get_height() * 0.5f) - (m_text.get_height() * 0.25f)));
		}

		void Tooltip::can_draw(const bool can_draw) noexcept
		{
			m_draw = can_draw;
		}

		const bool Tooltip::can_draw() const noexcept
		{
			return m_draw;
		}
	} // namespace ui
} // namespace galaxy