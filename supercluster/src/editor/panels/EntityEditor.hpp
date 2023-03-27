///
/// EntityEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_ENTITYEDITOR_HPP_

#include <imgui_internal.h>
#include <imgui_addons/font_awesome_5.h>

#include <galaxy/components/Script.hpp>
#include <galaxy/meta/Concepts.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>

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
			EntityEditor()  = default;
			~EntityEditor() = default;

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
					if constexpr (std::is_same<Component, components::Script>::value)
					{
						auto file = core::ServiceLocator<fs::VirtualFileSystem>::ref().show_open_dialog("*.lua");
						if (file.has_value())
						{
							auto& script = selected.m_world->m_registry.emplace<components::Script>(selected.m_selected);
							script.load(file.value());
						}
					}
					else
					{
						selected.m_world->m_registry.emplace<Component>(selected.m_selected);
					}

					ImGui::CloseCurrentPopup();
				}
			}
		}

		template<meta::valid_component Component, typename Func>
		inline void EntityEditor::draw_component(Selected& selected, const std::string& name, Func&& func)
		{
			const constexpr auto spacing             = ImVec2 {4, 4};
			const constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
													   ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			auto component = selected.m_world->m_registry.try_get<Component>(selected.m_selected);
			if (component)
			{
				auto content_region = ImGui::GetContentRegionAvail();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, spacing);

				const auto line_height = ImGui::GetCurrentContext()->Font->FontSize + ImGui::GetCurrentContext()->Style.FramePadding.y * 2.0f;

				ImGui::Separator();
				const auto hash = std::to_string(typeid(Component).hash_code());

				auto open = ImGui::TreeNodeEx(hash.c_str(), flags, name.c_str());
				auto pop  = false;

				ImGui::PopStyleVar();
				ImGui::SameLine(content_region.x - line_height * 0.5f);

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
				const auto pressed = ImGui::Button(ICON_FA_COG);

				if (pressed)
				{
					ImGui::OpenPopup("ComponentsContextMenu");
				}

				if (ImGui::BeginPopup("ComponentsContextMenu"))
				{
					if (ImGui::MenuItem("Remove Component##"))
					{
						selected.m_world->m_registry.remove<Component>(selected.m_selected);
						ImGui::CloseCurrentPopup();

						open = false;
						pop  = true;
					}

					ImGui::EndPopup();
				}

				if (open)
				{
					func(component);
					ImGui::TreePop();
				}

				if (pop)
				{
					ImGui::TreePop();
					pop = false;
				}
			}
		}
	} // namespace panel
} // namespace sc

#endif