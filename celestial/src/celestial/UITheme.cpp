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
	UITheme::UITheme(const int atlasSize, qs::Window* window, qs::Renderer* renderer) noexcept
		:m_atlas(atlasSize), m_window(window), m_renderer(renderer)
	{
	}

	UITheme::~UITheme() noexcept
	{
		m_fonts.clear();
		m_window = nullptr;
		m_renderer = nullptr;
	}

	void UITheme::create(qs::Shader& shader, const std::vector<std::string>& textures, const std::vector<celestial::FontData>& fonts)
	{
		for (auto& texture : textures)
		{
			m_atlas.add(texture);
		}

		m_atlas.create(*m_window, *m_renderer, shader);

		for (auto& fontData : fonts)
		{
			auto fp = std::filesystem::path(fontData.first);
			m_fonts.emplace(fp.stem().string(), qs::Font { fp.string(), fontData.second });
		}
	}

	qs::Font* UITheme::getFont(const std::string& key)
	{
		// at() for exception checking.
		return &(m_fonts.at(key));
	}

	qs::Window* UITheme::getWindow() const noexcept
	{
		return m_window;
	}

	qs::Renderer* UITheme::getRenderer() const noexcept
	{
		return m_renderer;
	}
}