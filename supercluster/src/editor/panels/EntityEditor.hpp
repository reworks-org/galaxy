///
/// EntityEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_

#include <optional>

#include <robin_hood.h>

#include <galaxy/components/Sprite.hpp>
#include <galaxy/core/Scene.hpp>

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		///
		/// Parses EntityEditor and provides imgui widgets to manipulate it.
		///
		class EntityEditor final
		{
		public:
			EntityEditor();
			~EntityEditor();

			void pre_render();
			void render();

			void set_scene(core::Scene* scene);

		private:
			void render_components(const ecs::Entity entity);

		private:
			core::Scene* m_cur_scene;
			std::optional<ecs::Entity> m_selected;
		};
	} // namespace panel
} // namespace sc

#endif