//
//  RenderSystem.hpp
//  rework
//
//  Created by reworks on 6/08/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_RENDERSYSTEM_HPP_
#define REWORK_RENDERSYSTEM_HPP_

#include "re/core/World.hpp"
#include "re/graphics/Layer.hpp"
#include "re/services/ServiceLocator.hpp"

namespace re
{
	class RenderSystem : public ex::System<RenderSystem>
	{
	public:
		///
		/// \brief Constructor.
		///
		/// Automatically registers system component functions.
		/// You need to manually register your components by calling registerComponetUpdateFunction()
		///
		/// \param layers Number of layers to draw to.
		/// \param defaultAlloc Minimum amount of space reserved in std::vector for entitys.
		///
		RenderSystem(unsigned int layers, unsigned int defaultAlloc = 20);

		///
		/// Destructor.
		///
		~RenderSystem() override;

		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by entity x internal system manager.
		///
		void update(ex::EntityManager& es, ex::EventManager& events, ex::TimeDelta dt);

		///
		/// Render entitys.
		///
		void render();

		///
		/// Clean up system.
		///
		void clean();

		///
		/// Function to register the updating of renderable components in layers.
		///
		/// \param T Type - Type of component to register for creation.
		///
		template<typename T>
		void registerComponentUpdateFunction()
		{
			m_cuf.emplace_back([this]() 
			{
				Locator::get<World>()->m_entityManager.each<T>([](ex::Entity& e, T& rc)
				{
					m_layers[rc.m_layer].insert(rc);
				};);
			});
		}

	private:
		unsigned int m_layerCount;
		unsigned int m_defaultAlloc;
		std::vector<Layer> m_layers;
		std::vector<std::function<void(void)>> m_cuf;
	};
}

#endif