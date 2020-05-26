///
/// SpriteBatchComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "SpriteBatchComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	SpriteBatchComponent::SpriteBatchComponent() noexcept
		:m_spritebatch(), m_atlas(nullptr)
	{
	}

	SpriteBatchComponent::SpriteBatchComponent(const nlohmann::json& json) noexcept
	{
	}

	SpriteBatchComponent::~SpriteBatchComponent() noexcept
	{
		m_atlas = nullptr;
	}

	void SpriteBatchComponent::setAtlas(qs::TextureAtlas* atlas) noexcept
	{
		m_atlas = atlas;
	}

	void SpriteBatchComponent::create(const nlohmann::json& json) noexcept
	{
		qs::VertexQuadStorage vqs;
		std::for_each(json.begin(), json.end(), [&](const nlohmann::json& arr)
		{
			auto& texQuad = m_atlas->getTexQuad(arr.at("atlasID"));
			auto quad = qs::Vertex::make_quad(
				{ 0.0f, 0.0f, texQuad.m_width, texQuad.m_height },
				{ 0.0f, 0.0f, 0.0f, 1.0f },
				texQuad.m_x, texQuad.m_y
			);
			vqs.push_back(quad);
		});

		m_spritebatch.create(vqs);
		
		auto counter = 0;
		std::for_each(json.begin(), json.end(), [&](const nlohmann::json& arr)
		{
			auto* tf = m_spritebatch.getTransform(counter);
			tf->setPos(arr.at("x"), arr.at("y"));

			counter++;
		});
	}
}