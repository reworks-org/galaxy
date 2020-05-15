///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/World.hpp>
#include <qs/renderer/Renderer.hpp>
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
			// Set initial transform.
			if (!tc->m_transform.isDefaultSet())
			{
				qs::VertexStorage* buff = &sc->getVBO().getVertexs();
				auto* vertex = &buff->at(0);

 				tc->m_transform.setTexels(vertex->m_texels[0], vertex->m_texels[1]);

				vertex = &buff->at(2);
				tc->m_transform.setRotationOrigin(vertex->m_position[0] * 0.5f, vertex->m_position[1] * 0.5f);
			}

			if (tc->m_transform.isDirty())
			{
				// Only update opacity and texels.
				// Position is done GPU side.
				qs::Vertex* vertex = nullptr;
				qs::VertexStorage* buff = &sc->getVBO().getVertexs();
				glm::vec3* texels = &tc->m_transform.getTexelTransform();

				vertex = &buff->at(0);
				vertex->m_texels[0] = texels->x;
				vertex->m_texels[1] = texels->y;
				vertex->m_colour[3] = texels->z;

				vertex = &buff->at(1);
				vertex->m_texels[0] = texels->x + sc->getWidth();
				vertex->m_texels[1] = texels->y;
				vertex->m_colour[3] = texels->z;

				vertex = &buff->at(2);
				vertex->m_texels[0] = texels->x + sc->getWidth();
				vertex->m_texels[1] = texels->y + sc->getHeight();
				vertex->m_colour[3] = texels->z;

				vertex = &buff->at(3);
				vertex->m_texels[0] = texels->x;
				vertex->m_texels[1] = texels->y + sc->getHeight();
				vertex->m_colour[3] = texels->z;

				glNamedBufferSubData(sc->getVBO().getID(), 0, buff->size() * sizeof(qs::Vertex), buff->data());
				tc->m_transform.setDirty(false);
			}
		});
	}

	void RenderSystem::render(galaxy::World* world, qs::Shader& shader)
	{
		auto* renderer = SL_HANDLE.renderer();
		world->operate<SpriteComponent, TransformComponent>([&](sr::Entity, SpriteComponent* sc, TransformComponent* tc)
		{
			renderer->drawRenderable(dynamic_cast<qs::Renderable*>(sc), tc->m_transform, shader);
		});
	}
}