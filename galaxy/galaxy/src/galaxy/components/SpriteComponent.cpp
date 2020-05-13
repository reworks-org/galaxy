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
		:Renderable()
	{
	}

	SpriteComponent::SpriteComponent(const nlohmann::json& json) noexcept
		:Renderable()
	{
		load(json.at("texture"));
		
		bool dynamic = json.at("dynamic-buffer");
		if (dynamic)
		{
			create<qs::BufferTypeDynamic>();
		}
		else
		{
			create<qs::BufferTypeStatic>();
		}
	}

	SpriteComponent::~SpriteComponent() noexcept
	{
		unbind();
		glDeleteTextures(1, &m_textureHandle);
	}
}