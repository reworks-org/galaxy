///
/// Tooltip.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include <sstream>

#include "Tooltip.hpp"

///
/// Core namespace.
///
namespace celestial
{
	Tooltip::Tooltip() noexcept
		:m_x(0.0f), m_y(0.0f), m_text(""), m_theme(nullptr), m_texture(nullptr)
	{
	}

	Tooltip::~Tooltip() noexcept
	{
		m_texture.reset();
	}

	void Tooltip::onMove(const protostar::MouseMovedEvent& e) noexcept
	{
		m_x = static_cast<double>(e.m_x + 12);
		m_y = static_cast<double>(e.m_y + 12);
	}

	void Tooltip::draw(celestial::interface::Renderer* renderer) noexcept
	{
		renderer->drawTexture(m_texture.get(), m_x, m_y);
		renderer->drawText(m_theme->getFont(), m_theme->getColour(), m_text, m_x, m_y);
	}

	void Tooltip::setTheme(celestial::UITheme* theme) noexcept
	{
		m_theme = theme;
	}

	void Tooltip::setText(const std::string& text) noexcept
	{
		m_text = text;
	}

	void Tooltip::setTexture(const std::string& texture) noexcept
	{
		std::stringstream stream(texture);

		char delim = '|';
		std::string name = "";
		std::string x_left = "";
		std::string x_right = "";

		std::getline(stream, name, delim);
		std::getline(stream, x_left, delim);
		std::getline(stream, x_right, delim);

		m_texture = m_theme->getLoader()->createSectionedTexture(m_theme->extractWidgetTexture(name).get(), std::stoi(x_left), std::stoi(x_right));
	}
}