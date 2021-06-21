///
/// GUIPanel.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imgui_stdlib.h>

#include "GUIPanel.hpp"

namespace sc
{
	namespace panel
	{
		void GUIPanel::gui_context_menu()
		{
			if (ImGui::BeginMenu("  GUI"))
			{
				if (ImGui::MenuItem("  Button"))
				{
				}

				if (ImGui::MenuItem("  Image"))
				{
				}

				if (ImGui::MenuItem("  Label"))
				{
				}

				if (ImGui::MenuItem("  ProgressBar"))
				{
				}

				if (ImGui::MenuItem("  Slider"))
				{
				}

				if (ImGui::MenuItem("  Textbox"))
				{
				}

				if (ImGui::MenuItem("  TextInput"))
				{
				}

				if (ImGui::MenuItem("  ToggleButton"))
				{
				}

				ImGui::EndMenu();
			}
		}

		void GUIPanel::render()
		{
		}
	} // namespace panel
} // namespace sc