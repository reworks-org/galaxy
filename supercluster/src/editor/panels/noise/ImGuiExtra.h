#pragma once

#include <imgui.h>
#include <imgui_internal.h>

namespace ImGuiExtra
{
	inline bool ScrollCombo(int* comboIndex, int comboCount)
	{
		if (ImGui::IsItemHovered())
		{
			if (ImGui::GetIO().MouseWheel < 0 && *comboIndex < comboCount - 1)
			{
				(*comboIndex)++;
				return true;
			}

			if (ImGui::GetIO().MouseWheel > 0 && *comboIndex > 0)
			{
				(*comboIndex)--;
				return true;
			}
		}
		return false;
	}
} // namespace ImGuiExtra
