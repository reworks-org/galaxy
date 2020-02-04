///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <solar/entity/Manager.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/TextureAtlas.hpp"
#include "galaxy/components/StateComponent.hpp"
#include "galaxy/components/SpriteComponent.hpp"
#include "galaxy/components/TransformComponent.hpp"

#include "RenderSystem.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	RenderSystem::RenderSystem() noexcept
		:m_verticies(sf::PrimitiveType::Quads, 0)
	{
		m_window = galaxy::ServiceLocator::i().window();
	}

	RenderSystem::~RenderSystem() noexcept
	{
		m_verticies.clear();
		m_window = nullptr;
	}

	void RenderSystem::render(galaxy::TextureAtlas* atlas)
	{
		m_window->draw(m_verticies, &atlas->getAtlas());
	}

	void RenderSystem::event(const sr::Event& e)
	{
	}

	void RenderSystem::update(const sr::DeltaTime time, sr::Manager& manager)
	{
		manager.operate<SpriteComponent, TransformComponent, StateComponent>([&](sr::Entity entity, SpriteComponent* sc, TransformComponent* tc, StateComponent* state)
			{
			
			});
	}
}