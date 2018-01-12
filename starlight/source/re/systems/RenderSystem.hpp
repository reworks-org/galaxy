///
/// RenderSystem.hpp
/// starlight
///
///  Created by reworks on 06/08/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_RENDERSYSTEM_HPP_
#define REWORK_RENDERSYSTEM_HPP_

#include "re/types/System.hpp"

namespace sl
{
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
		///
		RenderSystem(unsigned int layers, unsigned int defaultAlloc = 20, int quadtreeLayers = 5, int quadtreeMaxObjects = 10);

		///
		/// Destructor.
		///
		~RenderSystem() override;

		///
		/// Render entitys.
		///
		template<typename ... Components>
		void render();

		///
		/// Clean up system.
		///
		void clean();

		///
		/// Set current camera.
		///
		/// \param camera Camera to use.
		///
		void setCamera(Camera* camera);

		///
		/// Set current level.
		///
		/// \param level Level to use.
		///
		void setLevel(Level* level);

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
		QuadTree* m_quadtree;
		Camera* m_camera;
		Level* m_level;
		unsigned int m_layerCount;
		unsigned int m_defaultAlloc;
		int m_quadtreeLayers;
		int m_quadtreeMaxObjects;
		std::vector<Layer> m_layers;
		std::function<void(entityx::Entity&)> m_clf;

	};
}

#endif