///
/// ScenePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imgui_stdlib.h>
#include <magic_enum.hpp>

#include "ScenePanel.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		void ScenePanel::render(core::SceneStack& scene_stack)
		{
			if (ImGui::Begin("Scenes"))
			{
				if (ImGui::Button("New Scene"))
				{
					ImGui::OpenPopup("NewScenePopup");
				}

				if (ImGui::Button("Pop"))
				{
					scene_stack.pop();
				}

				if (ImGui::BeginPopup("NewScenePopup"))
				{
					static std::string s_buff = "";
					if (ImGui::InputText("New Scene", &s_buff, ImGuiInputTextFlags_EnterReturnsTrue))
					{
						scene_stack.create(s_buff);
						s_buff = "";
					}

					ImGui::EndPopup();
				}

				for (const auto& [name, scene] : scene_stack.get_scenes())
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
					if (m_selected == name)
					{
						flags |= ImGuiTreeNodeFlags_Selected;
					}

					if (m_selected.empty() || m_selected != name)
					{
						ImGui::SetNextItemOpen(false);
					}

					const bool is_open = ImGui::TreeNodeEx(name.c_str(), flags);
					if (ImGui::IsItemClicked())
					{
						m_selected = name;
					}

					if (is_open)
					{
						if (ImGui::Button("Push"))
						{
							scene_stack.push(name);
						}

						ImGui::TreePop();
					}

					if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
					{
						m_selected.clear();
					}
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc