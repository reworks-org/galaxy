///
/// RenderSystem.hpp
/// starlight
///
///  Created by reworks on 06/08/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RENDERSYSTEM_HPP_
#define STARLIGHT_RENDERSYSTEM_HPP_

#include "sl/types/System.hpp"

namespace sl
{
	class QuadTree;
	class RenderSystem : public System
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
		/// \param quadTreeLevels Maximum depth of a quadtree.
		/// \param quadtreeMaxObjects Maximim number of entitys in a node.
		///
		RenderSystem(unsigned int layers, unsigned int defaultAlloc = 20, int quadTreeLevels = 5, int quadtreeMaxObjects = 10);

		///
		/// Destructor.
		///
		~RenderSystem() override;

		///
		/// Clean up system.
		///
		void clean();

		void render(entt::DefaultRegistry& registery);

		///
		/// Retrieve number of rendering layers.
		///
		/// \return unsigned int CONST.
		///
		unsigned int getRenderingLayers() const;

	private:
		///
		/// Allocate internal layers.
		///
		void allocLayers();

		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by the world automatically.
		///
		void update(const double dt, entt::DefaultRegistry& registery) override;

		///
		/// Default Constructor.
		/// Deletd.
		///
		RenderSystem() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		RenderSystem(const RenderSystem&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		RenderSystem(RenderSystem&&) = delete;

	private:
		QuadTree m_quadtree;
		unsigned int m_layerCount;
		unsigned int m_defaultAlloc;
		std::vector<Layer> m_layers;
		std::vector<entt::Entity> m_entitys;
	};
}

#endif