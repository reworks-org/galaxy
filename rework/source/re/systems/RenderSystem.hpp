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
#include "re/utils/Utils.hpp"
#include "re/math/QuadTree.hpp"
#include "re/graphics/Layer.hpp"
#include "re/services/ServiceLocator.hpp"

namespace re
{
	class RenderSystem : public entityx::System<RenderSystem>
	{
	public:
		///
		/// \brief Constructor.
		///
		/// Automatically registers system component functions.
		/// You need to manually register your components by calling registerRenderableComponents()
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
		void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;

		///
		/// Render entitys.
		///
		void render();

		///
		/// Clean up system.
		///
		void clean();

		///
		/// Function to register the components that are renderable.
		/// Call this only once, with all components.
		///
		template<typename ... Components>
		void registerRenderableComponents()
		{
			m_clf = [this](entityx::Entity& e)
			{
				auto& tuple = e.components<Components...>();
				Utils::for_each_in_tuple(tuple, [this](auto &elem)
				{
					if (elem)
					{
						m_layers[elem->m_layer].insert(dynamic_cast<Renderable*>(elem.get()));
					}
				});
			};
		}

	private:
		QuadTree* m_quadtree;
		unsigned int m_layerCount;
		unsigned int m_defaultAlloc;
		std::vector<Layer> m_layers;
		std::function<void(entityx::Entity&)> m_clf;
	};
}

#endif