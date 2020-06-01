///
/// SpriteComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "SpriteComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	SpriteComponent::SpriteComponent() noexcept
		:VertexData(), Texture(), m_opacity(1.0f)
	{
	}

	void SpriteComponent::setOpacity(float opacity) noexcept
	{
		if (opacity > 1.0f)
		{
			opacity = 1.0f;
		}
		else if (opacity < 0.0f)
		{
			opacity = 0.0f;
		}

		m_opacity = opacity;
	}

	const float SpriteComponent::getOpacity() const noexcept
	{
		return m_opacity;
	}

	void SpriteComponent::bind() noexcept
	{
		m_vertexArray.bind();
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void SpriteComponent::unbind() noexcept
	{
		m_vertexArray.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	SpriteComponent::SpriteComponent(const nlohmann::json& json) noexcept
		:VertexData(), Texture(), m_opacity(1.0f)
	{
		load(json.at("texture"));
		setAnisotropy(json.at("ansio-filtering"));

		bool dynamic = json.at("dynamic-buffer");
		if (dynamic)
		{
			create<qs::BufferTypeDynamic>();
		}
		else
		{
			create<qs::BufferTypeStatic>();
		}

		m_opacity = json.at("opacity");
		m_zLevel = json.at("z-level");
	}
}