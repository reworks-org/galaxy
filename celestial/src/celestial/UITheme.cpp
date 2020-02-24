///
/// UITheme.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "UITheme.hpp"

///
/// Core namespace.
///
namespace celestial
{
	UITheme::UITheme() noexcept
		:m_resourceLoader(nullptr), m_font(nullptr), m_colour(0, 0, 0, 255), m_master(nullptr)
	{
	}

	UITheme::~UITheme() noexcept
	{
		m_font.reset();
		m_master.reset();

		// Cleanup, etc...
		m_widgetRegions.clear();

		m_resourceLoader = nullptr;
	}

	void UITheme::setResourceLoader(celestial::interface::ResourceLoader* loader) noexcept
	{
		m_resourceLoader = loader;
	}

	bool UITheme::setMasterTexture(const std::string& texture) noexcept
	{
		if (!m_resourceLoader)
		{
			return false;
		}
		else
		{
			m_master = m_resourceLoader->loadTexture(texture);
			return true;
		}
	}

	bool UITheme::setFont(const std::string& font) noexcept
	{
		if (!m_resourceLoader)
		{
			return false;
		}
		else
		{
			m_font = m_resourceLoader->loadFont(font);
			return true;
		}
	}
	
	void UITheme::setColour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a) noexcept
	{
		m_colour.m_red = r;
		m_colour.m_green = g;
		m_colour.m_blue = b;
		m_colour.m_alpha = a;
	}

	void UITheme::defineWidgetTexture(const std::string& id, const protostar::Rect<int>& dim) noexcept
	{
		m_widgetRegions.emplace(id, dim);
	}

	celestial::interface::TexturePtr UITheme::extractWidgetTexture(const std::string& id) noexcept
	{
		if (!m_resourceLoader)
		{
			return nullptr;
		}
		else
		{
			return m_resourceLoader->createSubTexture(m_master.get(), m_widgetRegions[id]);
		}
	}

	celestial::interface::Font* UITheme::getFont() const noexcept
	{
		return m_font.get();
	}

	const protostar::Colour& UITheme::getColour() const noexcept
	{
		return m_colour;
	}

	celestial::interface::ResourceLoader* UITheme::getLoader() const noexcept
	{
		return m_resourceLoader;
	}
}