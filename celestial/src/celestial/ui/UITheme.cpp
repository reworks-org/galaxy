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
	UITheme::UITheme(ResourceLoader* resourceLoader) noexcept
		:m_resourceLoader(resourceLoader), m_font(nullptr), m_colour(0, 0, 0, protostar::Colour::OPAQUE), m_master(nullptr)
	{
	}

	UITheme::~UITheme() noexcept
	{
		m_font.reset();
		m_master.reset();

		// Cleanup, etc...
		m_widgetRegions.clear();
	}

	void UITheme::setMasterTexture(const std::string& texture)
	{
		m_master = m_resourceLoader->loadTexture(texture);
	}

	void UITheme::setFont(const std::string& font)
	{
		m_font = m_resourceLoader->loadFont(font);
	}
	
	void UITheme::setColour(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a)
	{
		m_colour = protostar::Colour(r, g, b, a);
	}

	void UITheme::defineWidgetTexture(const std::string& id, const protostar::Rect<int>& dim) noexcept
	{
		m_widgetRegions.emplace(id, dim);
	}

	TexturePtr UITheme::extractWidgetTexture(const std::string& id) noexcept
	{
		protostar::Rect<int> region = m_widgetRegions[id];
		return m_resourceLoader->createSubTexture(m_master.get(), region);
	}

	const Font* UITheme::font() const noexcept
	{
		return m_font.get();
	}

	const protostar::Colour& UITheme::colour() const noexcept
	{
		return m_colour;
	}

	ResourceLoader* UITheme::loader() const noexcept
	{
		return m_resourceLoader;
	}
}