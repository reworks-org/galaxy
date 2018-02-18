///
/// RenderSystem.hpp
/// starlight
///
/// Created by reworks on 06/08/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RENDERSYSTEM_HPP_
#define STARLIGHT_RENDERSYSTEM_HPP_

#include "sl/types/System.hpp"

namespace sl
{
	class QuadTree;

	class RenderSystem final : public System
	{
	public:
		///
		/// Constructor.
		///
		/// \param quadTreeLevels Maximum depth of a quadtree.
		/// \param quadtreeMaxObjects Maximim number of entitys in a node.
		///
		RenderSystem(int quadTreeLevels = 5, int quadtreeMaxObjects = 10);

		///
		/// Destructor.
		///
		~RenderSystem() override;

		///
		/// Render entitys.
		///
		/// \param registry Default entity registry.
		/// 
		void render(entt::DefaultRegistry& registry);

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		RenderSystem() = delete;
		
		///
		/// Lets systems recieve events.
		///
		/// \param event ALLEGRO_EVENT passed by application class.
		///
		void event(ALLEGRO_EVENT* event, entt::DefaultRegistry& registry) override;

		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by the world automatically.
		///
		void update(const double dt, entt::DefaultRegistry& registry) override;

	private:
		QuadTree m_quadtree;
		unsigned int m_layerCount;
		unsigned int m_defaultAlloc;
		std::vector<entt::Entity> m_entitys;
	};
}

#endif