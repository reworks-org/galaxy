///
/// RenderSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <solar/entity/Manager.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "galaxy/flags/EnabledFlag.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/TextureAtlas.hpp"
#include "galaxy/components/SpriteComponent.hpp"
#include "galaxy/components/TransformComponent.hpp"

#include "RenderSystem.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	RenderSystem::RenderSystem() noexcept
		:m_verticies(sf::PrimitiveType::Quads, 0), m_stride(0)
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
		m_verticies.clear();
		m_stride = 0;

		manager.operate<SpriteComponent, TransformComponent, EnabledFlag>([&](const sr::Entity entity, SpriteComponent* sc, TransformComponent* tc, EnabledFlag* enabled)
		{
			if (m_stride + 4> m_verticies.getVertexCount())
			{
				m_verticies.resize(m_stride + 4);
			}

			m_verticies[m_stride].color = sc->m_colour;
			m_verticies[m_stride].position = tc->m_transform.transformPoint(0.0f, 0.0f);
			m_verticies[m_stride].texCoords = sf::Vector2f(sc->m_texRect.m_x, sc->m_texRect.m_y);

			m_verticies[m_stride + 1].color = sc->m_colour;
			m_verticies[m_stride + 1].position = tc->m_transform.transformPoint(0.0f, sc->m_texRect.m_height);
			m_verticies[m_stride + 1].texCoords = sf::Vector2f(sc->m_texRect.m_x, sc->m_texRect.m_height + sc->m_texRect.m_y);

			m_verticies[m_stride + 2].color = sc->m_colour;
			m_verticies[m_stride + 2].position = tc->m_transform.transformPoint(sc->m_texRect.m_width, sc->m_texRect.m_height);
			m_verticies[m_stride + 2].texCoords = sf::Vector2f(sc->m_texRect.m_x + sc->m_texRect.m_width, sc->m_texRect.m_y + sc->m_texRect.m_height);

			m_verticies[m_stride + 3].color = sc->m_colour;
			m_verticies[m_stride + 3].position = tc->m_transform.transformPoint(sc->m_texRect.m_width, 0.0f);
			m_verticies[m_stride + 3].texCoords = sf::Vector2f(sc->m_texRect.m_width + sc->m_texRect.m_x, sc->m_texRect.m_y);

			m_stride += 4;
		});
	}
}