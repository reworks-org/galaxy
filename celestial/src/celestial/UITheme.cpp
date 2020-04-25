///
/// UITheme.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include "UITheme.hpp"

///
/// Core namespace.
///
namespace celestial
{
	UITheme::UITheme() noexcept
		:m_atlas()
	{
	}

	UITheme::UITheme(const int atlasSize, const std::vector<std::string>& textures, const std::vector<celestial::FontData>& fonts)
		:m_atlas(atlasSize)
	{
		create(textures, fonts);
	}

	UITheme::~UITheme() noexcept
	{
		m_fonts.clear();
	}

	void UITheme::create(const std::vector<std::string>& textures, const std::vector<celestial::FontData>& fonts)
	{
		for (auto& texture : textures)
		{
			m_atlas.add(texture);
		}

		m_atlas.create(window, renderer, shader);

		for (auto& fontData : fonts)
		{
			auto fp = std::filesystem::path(fontData.first);
			m_fonts.emplace(fp.stem(), fp.string(), fontData.second);
		}
	}

	qs::Font* UITheme::getFont(const std::string& key)
	{
		return &(m_fonts[key]);
	}
}