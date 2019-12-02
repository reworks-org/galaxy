///
/// Tooltip.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include <sstream>

#include "Tooltip.hpp"

namespace celestial
{
	Tooltip::Tooltip(const std::string& text, const std::string& texture, UITheme* theme)
		:m_x(0), m_y(0), m_text(text), m_theme(theme)
	{
		std::stringstream stream(texture);

		char delim = '|';
		std::string name = "";
		std::string x_left = "";
		std::string x_right = "";

		std::getline(stream, name, delim);
		std::getline(stream, x_left, delim);
		std::getline(stream, x_right, delim);

		m_texture = m_theme->loader()->createSectionedTexture(m_theme->extractWidgetTexture(name), std::stoi(x_left), std::stoi(x_right));
	}

	Tooltip::Tooltip(const std::string& text, UITheme* theme)
		:m_x(0), m_y(0), m_text(text), m_theme(theme)
	{
		m_texture = m_theme->loader()->createRectangle(m_theme->loader()->getTextWidth(text), m_theme->loader()->getTextHeight(text), m_theme->colour());
	}

	Tooltip::~Tooltip() noexcept
	{
		m_texture.reset();
	}

	void Tooltip::receive(const celestial::MouseMovedEvent& e)
	{
		m_x = e.m_x + 12;
		m_y = e.m_y + 12;
	}

	void Tooltip::draw(celestial::compat::Renderer* renderer)
	{
		renderer->drawTexture(m_texture.get(), m_x, m_y);
		renderer->drawText(m_theme->font(), m_theme->colour(), m_text, m_x, m_y);
	}
}