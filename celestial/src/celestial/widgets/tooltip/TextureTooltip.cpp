///
/// TextureTooltip.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include <sstream>

///
/// Core namespace.
///
namespace celestial
{
	TextureTooltip::TextureTooltip(const std::string& text, const std::string& texture, UITheme* theme)
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

		m_texture = m_theme->loader()->createSectionedTexture(m_theme->extractWidgetTexture(name).get(), std::stoi(x_left), std::stoi(x_right));
	}

	TextureTooltip::TextureTooltip(const std::string& text, UITheme* theme)
		:m_x(0), m_y(0), m_text(text), m_theme(theme)
	{
		m_texture = m_theme->loader()->createRectangle(m_theme->loader()->getTextWidth(m_theme->font(), text), m_theme->loader()->getTextHeight(m_theme->font(), text), m_theme->colour());
	}

	TextureTooltip::TextureTooltip() noexcept
	{
	}

	TextureTooltip::~TextureTooltip() noexcept
	{
		m_texture.reset();
	}

	void TextureTooltip::onMove(const protostar::MouseMovedEvent& e)
	{
	}

	void TextureTooltip::receive(const protostar::MouseMovedEvent& e)
	{
		m_x = e.m_x + 12;
		m_y = e.m_y + 12;
	}

	void TextureTooltip::draw(celestial::Renderer* renderer)
	{
		renderer->drawTexture(m_texture.get(), m_x, m_y);
		renderer->drawText(m_theme->font(), m_theme->colour(), m_text, m_x, m_y);
	}

	void TextureTooltip::setTheme(celestial::UITheme* theme) noexcept
	{
		m_theme = theme;
	}

	void TextureTooltip::setText(const std::string& text) noexcept
	{
		m_text = text;
	}
}