///
/// Image.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Shader.hpp>

#include "Image.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Image::Image(galaxy::Theme* theme, std::string_view textureID, const float x, const float y) noexcept
		:Widget(theme)
	{
		auto* atlas = m_theme->getAtlas();
		auto* batch = m_theme->getBatch();
		m_sprite.create(atlas->getTexQuad(textureID), 0);
		m_sprite.setPos(x, y);

		batch->add(&m_sprite);
	}

	Image::~Image() noexcept
	{
	}

	void Image::activate() noexcept
	{
	}

	void Image::deactivate() noexcept
	{
	}

	void Image::update(pr::ProtectedDouble* dt) noexcept
	{
	}
	
	void Image::render(qs::Camera& camera) noexcept
	{
	}
}