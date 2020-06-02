///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Renderer.hpp>
#include <galaxy/core/World.hpp>
#include <galaxy/core/ServiceLocator.hpp>

#include "galaxy/components/SpriteComponent.hpp"
#include "galaxy/components/TransformComponent.hpp"

#include "RenderSystem.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	RenderSystem::RenderSystem() noexcept
	{
	}

	RenderSystem::~RenderSystem() noexcept
	{
	}

	void RenderSystem::events() noexcept
	{
	}

	void RenderSystem::update(protostar::ProtectedDouble* deltaTime, sr::Manager& manager) noexcept
	{
		manager.operate<SpriteComponent, TransformComponent>([](sr::Entity, SpriteComponent* sc, TransformComponent* tc)
		{
		});
	}

	void RenderSystem::render(galaxy::World* world, qs::Shader& shader)
	{
		auto* renderer = SL_HANDLE.renderer();
		world->operate<SpriteComponent, TransformComponent>([&](sr::Entity, SpriteComponent* sc, TransformComponent* tc)
		{
		});
	}
}