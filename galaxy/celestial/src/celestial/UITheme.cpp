///
/// UITheme.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <pulsar/Log.hpp>

#include "UITheme.hpp"

///
/// Core namespace.
///
namespace celestial
{
	UITheme::UITheme(const int atlasSize, qs::Window* window, qs::Renderer* renderer, qs::Shader* textShader) noexcept
		:m_atlas(atlasSize), m_window(window), m_renderer(renderer), m_textShader(textShader)
	{
	}

	UITheme::~UITheme() noexcept
	{
		m_fonts.clear();
		m_window = nullptr;
		m_renderer = nullptr;
		m_textShader = nullptr;
	}

	void UITheme::create(qs::Shader& shader, const std::vector<std::string>& textures, const std::vector<celestial::FontData>& fonts) noexcept
	{
		for (auto& texture : textures)
		{
			m_atlas.add(texture);
		}

		m_atlas.create(*m_window, *m_renderer, shader);

		for (auto& fontData : fonts)
		{
			if (!fontData.first.empty())
			{
				m_fonts.emplace(fontData.first, fontData.second);
			}
			else
			{
				PL_LOG(PL_WARNING, "Font data path empty, skipping...");
			}
		}
	}

	qs::Shader* UITheme::getTextShader() noexcept
	{
		return m_textShader;
	}

	qs::Font* UITheme::getFont(const std::string& key) noexcept
	{
		if (m_fonts.find(key) != m_fonts.end())
		{
			return m_fonts[key];
		}
		else
		{
			PL_LOG(PL_ERROR, "Tried to access font that does not exist!");
			return nullptr;
		}
	}

	qs::Window* UITheme::getWindow() const noexcept
	{
		return m_window;
	}

	qs::Renderer* UITheme::getRenderer() const noexcept
	{
		return m_renderer;
	}

	qs::TextureAtlas* UITheme::getAtlas() noexcept
	{
		return &m_atlas;
	}
}