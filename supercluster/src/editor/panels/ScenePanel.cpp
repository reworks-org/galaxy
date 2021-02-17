///
/// ScenePanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imgui_stdlib.h>

#include "ScenePanel.hpp"

namespace sc
{
	namespace panel
	{
		void ScenePanel::render()
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
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc