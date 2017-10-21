// ImGui Allegro 5 bindings
// In this binding, ImTextureID is used to store a 'ALLEGRO_BITMAP*' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui
// by @birthggd

#pragma once

struct ALLEGRO_DISPLAY;
union ALLEGRO_EVENT;

#include <vector>

#include "imgui/imgui.h"

IMGUI_API bool    ImGui_ImplA5_Init(ALLEGRO_DISPLAY* display);
IMGUI_API void    ImGui_ImplA5_Shutdown();
IMGUI_API void    ImGui_ImplA5_NewFrame();
IMGUI_API bool    ImGui_ImplA5_ProcessEvent(ALLEGRO_EVENT* event);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API bool    Imgui_ImplA5_CreateDeviceObjects();
IMGUI_API void    ImGui_ImplA5_InvalidateDeviceObjects();

// Allows use of STL types in some widgets
// This is not originally part of this file.
// Credits for code:
// https://github.com/simongeilfus/Cinder-ImGui
namespace ImGui
{
	namespace stl
	{
		inline bool ListBox(const char* label, int* current_item, const std::vector<std::string>& items, int height_in_items = -1)
		{
			// copy names to a vector
			std::vector<const char*> names;
			for (auto item : items) {
				char *cname = new char[item.size() + 1];
				std::strcpy(cname, item.c_str());
				names.push_back(cname);
			}

			bool result = ImGui::ListBox(label, current_item, names.data(), names.size(), height_in_items);
			// cleanup
			for (auto &name : names) delete[] name;
			return result;
		}

		inline bool InputText(const char* label, std::string* buf, ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL)
		{
			// conversion
			char *buffer = new char[buf->size() + 128];
			std::strcpy(buffer, buf->c_str());
			bool result = ImGui::InputText(label, buffer, buf->size() + 128, flags, callback, user_data);
			if (result) {
				*buf = std::string(buffer);
			}
			// cleanup
			delete[] buffer;
			return result;
		}

		inline bool InputTextMultiline(const char* label, std::string* buf, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL)
		{
			// conversion
			char *buffer = new char[buf->size() + 128];
			std::strcpy(buffer, buf->c_str());
			bool result = ImGui::InputTextMultiline(label, buffer, buf->size() + 128, size, flags, callback, user_data);
			if (result) {
				*buf = std::string(buffer);
			}
			// cleanup
			delete[] buffer;
			return result;
		}

		inline bool Combo(const char* label, int* current_item, const std::vector<std::string>& items, int height_in_items = -1)
		{
			// conversion
			std::string itemsNames;
			for (auto item : items)
				itemsNames += item + '\0';
			itemsNames += '\0';

			std::vector<char> charArray(itemsNames.begin(), itemsNames.end());
			bool result = ImGui::Combo(label, current_item, (const char*)&charArray[0], height_in_items);
			return result;
		}
	}
}