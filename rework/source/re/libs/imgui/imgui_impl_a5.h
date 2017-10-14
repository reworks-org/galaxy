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

IMGUI_API bool    ImGui_ImplA5_Init(ALLEGRO_DISPLAY* display);
IMGUI_API void    ImGui_ImplA5_Shutdown();
IMGUI_API void    ImGui_ImplA5_NewFrame();
IMGUI_API bool    ImGui_ImplA5_ProcessEvent(ALLEGRO_EVENT* event);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API bool    Imgui_ImplA5_CreateDeviceObjects();
IMGUI_API void    ImGui_ImplA5_InvalidateDeviceObjects();

#include <vector>

// Allows use of std::vector in some widgets.
// This is not originally part of this file.
namespace imgui
{
	namespace vec 
	{
		static auto vector_getter = [](void* vec, int idx, const char** out_text)
		{
			auto& vector = *static_cast<std::vector<std::string>*>(vec);
			if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
			*out_text = vector.at(idx).c_str();
			return true;
		};

		inline bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
		{
			if (values.empty()) { return false; }
			return ImGui::Combo(label, currIndex, vector_getter,
				static_cast<void*>(&values), values.size());
		}

		inline bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
		{
			if (values.empty()) { return false; }
			return ImGui::ListBox(label, currIndex, vector_getter,
				static_cast<void*>(&values), values.size());
		}
	}
}