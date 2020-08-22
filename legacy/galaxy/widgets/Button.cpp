///
/// Button.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Renderer.hpp>
#include <galaxy/res/FontBook.hpp>
#include <galaxy/res/ShaderBook.hpp>

#include "Button.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Button::Button(galaxy::Theme* theme, const int x, const int y) noexcept
		:Widget(theme)
	{
		auto* atlas = m_theme->getAtlas();
		auto* batch = m_theme->getBatch();
		auto* quad = &atlas->getTexQuad("button_default");
		
		m_sprite.create(*quad, 0);
		m_sprite.setPos(x, y);
		
		m_bounds.m_x = x;
		m_bounds.m_y = y;
		m_bounds.m_width = quad->m_width;
		m_bounds.m_height = quad->m_height;

		batch->add(&m_sprite);
	}

	Button::~Button() noexcept
	{
		m_text.reset();
	}

	void Button::onPress(const pr::MousePressedEvent& e) noexcept
	{
		if (m_bounds.contains(e.m_x, e.m_y) && e.m_button == 1)
		{
			m_state = Button::State::PRESSED;
		}
	}

	void Button::onRelease(const pr::MouseReleasedEvent& e) noexcept
	{
		if (m_bounds.contains(e.m_x, e.m_y))
		{
			m_state = Button::State::HOVER;
		}
		else
		{
			m_state = Button::State::DEFAULT;
		}
	}

	void Button::onMove(const pr::MouseMovedEvent& e) noexcept
	{
		if (m_bounds.contains(e.m_x, e.m_y))
		{
			m_state = Button::State::HOVER;
		}
		else
		{
			m_state = Button::State::DEFAULT;
		}
	}

	void Button::createLabel(const std::string& label, std::string_view font, const pr::Colour& col) noexcept
	{
		m_text = std::make_unique<qs::Text>();
		m_text->load(label, m_theme->get<FontBook>(font), col);
		m_text->create();
	}

	void Button::activate() noexcept
	{
	}

	void Button::deactivate() noexcept
	{
	}

	void Button::update(pr::ProtectedDouble* dt) noexcept
	{
		if (m_state == Button::State::PRESSED)
		{
			m_pressed = true;
		}
		else
		{
			m_pressed = false;
		}
	}

	void Button::render(qs::Camera& camera) noexcept
	{
		if (m_text != nullptr)
		{
			auto* shader = m_theme->get<ShaderBook>("text");
			shader->setUniform<glm::mat4>("u_cameraProj", camera.getProj());
			shader->setUniform<glm::mat4>("u_cameraView", camera.getTransformation());

			m_theme->getRenderer()->drawText(*m_text, *shader);
		}
	}

	const bool Button::isPressed() const noexcept
	{
		return false;
	}
}