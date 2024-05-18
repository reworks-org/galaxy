///
/// EntityPanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_PANELS_ENTITYPANEL_HPP_
#define SUPERCLUSTER_PANELS_ENTITYPANEL_HPP_

#include <imgui/combo_filter/imgui_combo_autoselect.h>
#include <imgui/imgui_internal.h>
#include <imgui/imnotify/material_design_icons.h>
#include <nlohmann/json.hpp>

#include <galaxy/components/Script.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>
#include <galaxy/utils/StringUtils.hpp>

#include "../Selected.hpp"

using namespace galaxy;

namespace sc
{
	class EntityPanel final
	{
	  public:
		EntityPanel();
		~EntityPanel();

		void render(meta::vector<std::move_only_function<void(void)>>& tasks, Selected& selected);

	  private:
		template<meta::valid_component Component>
		void draw_entry(Selected& selected, const std::string& name);

		template<meta::valid_component Component, typename Func>
		void draw_component(Selected& selected, const std::string& name, Func&& func);

	  public:
		bool m_show = true;

	  private:
		bool m_open_flags      = false;
		bool m_open_components = false;

		ImGui::ComboAutoSelectData m_texture_data;
		ImGui::ComboAutoSelectData m_alignment_data;
		ImGui::ComboAutoSelectData m_font_data;
	};

	template<meta::valid_component Component>
	inline void EntityPanel::draw_entry(Selected& selected, const std::string& name)
	{
		if (!selected.scene->m_registry.m_entt.all_of<Component>(selected.entity))
		{
			// TODO: is this supposed to be button?
			if (ImGui::MenuItem(name.c_str()))
			{
				if constexpr (std::is_same<Component, components::Script>::value)
				{
					const auto path = core::ServiceLocator<fs::VirtualFileSystem>::ref().open_file_dialog({"*.lua"});
					if (!path.empty())
					{
						auto str = "{\"file\":\"" + path + "\"}";
						strutils::replace_all(str, "\\", "/");
						selected.scene->m_registry.m_entt.emplace<components::Script>(selected.entity, nlohmann::json::parse(str));
					}
				}
				else
				{
					selected.scene->m_registry.m_entt.emplace<Component>(selected.entity);
				}

				ImGui::CloseCurrentPopup();
			}
		}
	}

	template<meta::valid_component Component, typename Func>
	inline void EntityPanel::draw_component(Selected& selected, const std::string& name, Func&& func)
	{
		const constexpr auto               spacing = ImVec2 {4, 4};
		const constexpr ImGuiTreeNodeFlags flags   = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
												   ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_FramePadding;

		auto component = selected.scene->m_registry.m_entt.try_get<Component>(selected.entity);
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
			const auto pressed = ImGui::Button(ICON_MDI_COG);

			if (pressed)
			{
				ImGui::OpenPopup("ComponentsContextMenu");
			}

			if (ImGui::BeginPopup("ComponentsContextMenu"))
			{
				if (ImGui::MenuItem("Remove Component##"))
				{
					selected.scene->m_registry.m_entt.remove<Component>(selected.entity);
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
} // namespace sc

#endif
