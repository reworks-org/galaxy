///
/// Theme.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Shader.hpp>

#include "Theme.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Theme::Theme() noexcept
		:m_window(nullptr), m_renderer(nullptr), m_spriteBatch(1000)
	{
		m_spriteBatch.setTexture(m_atlas.getTexture());
	}

	Theme::~Theme() noexcept
	{
		m_caches.clear();
	}

	void Theme::setWindow(qs::Window* window) noexcept
	{
		m_window = window;
	}

	void Theme::setRenderer(qs::Renderer* renderer) noexcept
	{
		m_renderer = renderer;
	}

	void Theme::addTextureToAtlas(const std::string& texture) noexcept
	{
		m_atlas.add(texture);
	}

	void Theme::createTextureAtlas(qs::Shader& shader) noexcept
	{
		shader.bind();
		m_atlas.create(*m_renderer, shader);
	}

	qs::TextureAtlas* Theme::getAtlas() noexcept
	{
		return &m_atlas;
	}

	qs::SpriteBatch* Theme::getBatch() noexcept
	{
		return &m_spriteBatch;
	}

	qs::Window* Theme::getWindow() const noexcept
	{
		return m_window;
	}

	qs::Renderer* Theme::getRenderer() const noexcept
	{
		return m_renderer;
	}
}