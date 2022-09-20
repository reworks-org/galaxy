///
/// EntityEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_

#include <imgui_internal.h>

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
			void draw_entry(Selected& selected, const std::string& name);

			template<meta::valid_component Component, typename Func>
			void draw_component(Selected& selected, const std::string& name, Func&& func);
		};

		template<meta::valid_component Component>
		inline void EntityEditor::draw_entry(Selected& selected, const std::string& name)
		{
			if (!selected.m_world->m_registry.all_of<Component>(selected.m_selected))
			{
				if (ImGui::MenuItem(name.c_str()))
				{
					selected.m_world->m_registry.emplace<Component>(selected.m_selected);
					ImGui::CloseCurrentPopup();
				}
			}
		}

		template<meta::valid_component Component, typename Func>
		inline void EntityEditor::draw_component(Selected& selected, const std::string& name, Func&& func)
		{
			const constexpr auto spacing             = ImVec2 {4, 4};
			const constexpr ImGuiTreeNodeFlags flags = /* ImGuiTreeNodeFlags_DefaultOpen |*/ ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
													   ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			auto component = selected.m_world->m_registry.try_get<Component>(selected.m_selected);
			if (component)
			{
				auto content_region = ImGui::GetContentRegionAvail();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, spacing);

				const auto line_height = ImGui::GetCurrentContext()->Font->FontSize + ImGui::GetCurrentContext()->Style.FramePadding.y * 2.0f;

				ImGui::Separator();
				const auto hash = std::to_string(typeid(Component).hash_code());
				const auto open = ImGui::TreeNodeEx(hash.c_str(), flags, name.c_str());

				ImGui::PopStyleVar();
				ImGui::SameLine(content_region.x - line_height * 0.5f);

				const auto id = "-##" + hash;
				if (ImGui::Button(id.c_str(), ImVec2 {line_height, line_height}))
				{
					ImGui::OpenPopup("ComponentsContextMenu");
				}

				if (ImGui::BeginPopup("ComponentsContextMenu"))
				{
					const auto mi_id = "Remove Component##" + hash;
					if (ImGui::MenuItem(mi_id.c_str()))
					{
						selected.m_world->m_registry.remove<Component>(selected.m_selected);
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

				if (open)
				{
					func(component);
					ImGui::TreePop();
				}
			}
		}
	} // namespace panel
} // namespace sc

#endif