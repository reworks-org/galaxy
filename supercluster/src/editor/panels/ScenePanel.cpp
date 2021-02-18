///
/// ScenePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imgui_stdlib.h>

#include "ScenePanel.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		void ScenePanel::render(Scenemap& scene_map)
		{
			if (ImGui::Begin("Scenes"))
			{
				if (ImGui::ArrowButton("PlaySceneArrowButton", ImGuiDir_Right))
				{
				}

				ImGui::SameLine();

				if (ImGui::Button(" || ##PauseSceneButton"))
				{
				}

				ImGui::Spacing();

				ImGui::Button("New Scene");

				ImGui::Separator();

				for (const auto& [name, scene] : scene_map)
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