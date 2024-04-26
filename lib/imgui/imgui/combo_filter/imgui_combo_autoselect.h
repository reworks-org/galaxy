#pragma once

#include <string>
#include <vector>

#include "../imgui.h"
#include "../imgui_internal.h"

namespace ImGui
{
	struct ComboAutoSelectData
	{
		std::vector<std::string> items;
		int                      index      = -1;
		char                     input[128] = {};

		ComboAutoSelectData()
		{
		}

		ComboAutoSelectData(std::vector<std::string>&& hints, int selected_index = -1)
			: items(hints)
		{
			if (selected_index > -1 && selected_index < (int)items.size())
			{
				strncpy(input, items[selected_index].c_str(), sizeof(input) - 1);
				index = selected_index;
			}
		}
	};

	bool ComboAutoSelect(const char* label, ComboAutoSelectData& data, ImGuiComboFlags flags = 0);
} // namespace ImGui
