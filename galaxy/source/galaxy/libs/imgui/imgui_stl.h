// imgui_stl.h
// From: https://github.com/Csabix/imgui/tree/master/auto main repo. All rights reserved to them.

#pragma once

#include <string>
#include <vector>

#include "imgui.h"

namespace ImGui
{
bool InputText(const char* label, std::string &buf, ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
//IMGUI_API bool InputText(const char* label, std::vector<char>& buf, ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
bool InputTextMultiline(const char* label, std::string& buf, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
//IMGUI_API bool InputTextMultiline(const char* label, std::vector<char>& buf, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);
bool ListBox(const char* label, int* current_item, const std::vector<std::string>& itemNames, int height_in_items = -1);
bool Combo(const char* label, int* current_item, const std::vector<std::string>& itemNames, int height_in_items = -1);
void PlotLines(const char* label, const std::vector<float> &values, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride_in_floats = 1);
void PlotHistogram(const char* label, const std::vector<float> &values, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride_in_floats = 1);
//Plot a Real function using a lambda
template<typename Lambda> void PlotLines(const char* label, const Lambda &fun, const ImVec2 &domain, const ImVec2 &range = ImVec2(FLT_MAX, FLT_MAX), int values_count = 64,  const char* overlay_text = NULL, ImVec2 graph_size = ImVec2(0, 0))
{
	struct UserData
	{
		const Lambda *function;
		float domain_width;
		float domain_low;
		float max_idx;
	} user_data = {&fun, domain.y - domain.x, domain.x, static_cast<float>(values_count-1)};
	PlotLines(label,
			  [](void*data, int idx)->float
			  {
				  const auto &user_data = *static_cast<UserData*>(data);
				  return (*user_data.function)(user_data.domain_width * static_cast<float>(idx)/ user_data.max_idx + user_data.domain_low);
			  },
			  static_cast<void*>(&user_data), values_count, 0, overlay_text, range.x, range.y, graph_size);
}
//Plot a Real function using a lambda
template<typename Lambda> void PlotHistogram(const char* label, const Lambda &fun, const ImVec2 &domain, const ImVec2 &range = ImVec2(FLT_MAX, FLT_MAX), int values_count = 16, const char* overlay_text = NULL, ImVec2 graph_size = ImVec2(0, 0))
{
	struct UserData
	{
		const Lambda *function;
		float domain_width;
		float domain_low;
		float max_idx;
	} user_data = { &fun, domain.y - domain.x, domain.x, static_cast<float>(values_count - 1) };
	PlotHistogram(label,
				  [](void*data, int idx)->float
				  {
					  const auto &user_data = *static_cast<UserData*>(data);
					  return (*user_data.function)(user_data.domain_width * static_cast<float>(idx) / user_data.max_idx + user_data.domain_low);
				  },
				  static_cast<void*>(&user_data), values_count, 0, overlay_text, range.x, range.y, graph_size);
}

}