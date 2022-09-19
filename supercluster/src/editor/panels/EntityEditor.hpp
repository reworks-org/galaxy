///
/// EntityEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_

#include <galaxy/meta/Concepts.hpp>

#include "editor/Selected.hpp"
#include "editor/UpdateStack.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		class EntityEditor final
		{
		public:
			EntityEditor() noexcept;
			~EntityEditor() noexcept;

			void render(Selected& selected, UpdateStack& updates);

		private:
			template<meta::valid_component Component>
			void draw_entry(Selected& selected, std::string_view name);
		};

		template<meta::valid_component Component>
		inline void EntityEditor::draw_entry(Selected& selected, std::string_view name)
		{
			if (!selected.m_world->m_registry.all_of<Component>(selected.m_selected))
			{
				if (ImGui::MenuItem(name.data()))
				{
					selected.m_world->m_registry.emplace<Component>(selected.m_selected);
					ImGui::CloseCurrentPopup();
				}
			}
		}
	} // namespace panel
} // namespace sc

#endif