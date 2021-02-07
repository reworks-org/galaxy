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
		    : m_draw {false}, m_theme {nullptr}, m_sprite_shader {nullptr}, m_text_shader {nullptr}
		{
		}

		Tooltip::Tooltip(Tooltip&& t) noexcept
		{
			this->m_draw             = t.m_draw;
			this->m_sprite           = std::move(t.m_sprite);
			this->m_text             = std::move(t.m_text);
			this->m_sprite_transform = std::move(t.m_sprite_transform);
			this->m_text_transform   = std::move(t.m_text_transform);
			this->m_theme            = t.m_theme;
			this->m_sprite_shader    = t.m_sprite_shader;
			this->m_text_shader      = t.m_text_shader;
		}

		Tooltip& Tooltip::operator=(Tooltip&& t) noexcept
		{
			if (this != &t)
			{
				this->m_draw             = t.m_draw;
				this->m_sprite           = std::move(t.m_sprite);
				this->m_text             = std::move(t.m_text);
				this->m_sprite_transform = std::move(t.m_sprite_transform);
				this->m_text_transform   = std::move(t.m_text_transform);
				this->m_theme            = t.m_theme;
				this->m_sprite_shader    = t.m_sprite_shader;
				this->m_text_shader      = t.m_text_shader;
			}

			return *this;
		}

		Tooltip::~Tooltip() noexcept
		{
			m_theme         = nullptr;
			m_sprite_shader = nullptr;
			m_text_shader   = nullptr;
		}

		void Tooltip::create(std::string_view sprite, std::string_view text, std::string_view font)
		{
			m_sprite.load(sprite);
			m_sprite.create();

			m_text.load(font, m_theme->m_font_col);
			m_text.create(text);

			m_sprite_transform.set_rotation_origin(m_sprite.get_width() * 0.5f, m_sprite.get_height() * 0.5f);
			m_text_transform.set_rotation_origin(m_text.get_width() * 0.5f, m_text.get_height() * 0.5f);

			m_sprite_shader = SL_HANDLE.shaderbook()->get("sprite");
			m_text_shader   = SL_HANDLE.shaderbook()->get("text");
		}

		void Tooltip::render(graphics::Camera& camera)
		{
			m_sprite_shader->bind();
			m_sprite_shader->set_uniform("u_cameraProj", camera.get_proj());
			m_sprite_shader->set_uniform("u_cameraView", camera.get_transform());
			graphics::Renderer::submit_sprite(&m_sprite, &m_sprite_transform, m_sprite_shader);

			m_text_shader->bind();
			m_text_shader->set_uniform("u_cameraProj", camera.get_proj());
			m_text_shader->set_uniform("u_cameraView", camera.get_transform());
			graphics::Renderer::submit_text(&m_text, &m_text_transform, m_text_shader);
		}

		void Tooltip::update_text(std::string_view text)
		{
			m_text.update_text(text);
		}

		void Tooltip::update_pos(const double x, const double y) noexcept
		{
			m_sprite_transform.set_pos(x, y);

			float text_x = 0.0f, text_y = 0.0f;
			text_x = x + ((m_sprite.get_width() * 0.5f) - (m_text.get_width() * 0.5f));
			text_y = y + ((m_sprite.get_height() * 0.5f) - (m_text.get_height() * 0.5f));
			m_text_transform.set_pos(text_x, text_y);
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