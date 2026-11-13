///
/// ImGuiHelpers.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <gl2d.h>
#include <glad/glad.h>

#include <entt/locator/locator.hpp>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/misc/freetype/imgui_freetype.h>
#include <imgui/notify/ImGuiNotify.hpp>
#include <mimalloc.h>

#include "galaxy/core/Settings.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/platform/Pragma.hpp"
#include "galaxy/resource/embedded/material_design_icons.h"
#include "galaxy/resource/embedded/material_design_icons_ttf.h"
#include "galaxy/resource/embedded/RobotoLight.hpp"

#include "ImGuiHelpers.hpp"

// #ifdef GALAXY_WIN_PLATFORM
// #pragma warning(push)
//  #pragma warning(disable : 4312)
// #endif

namespace galaxy
{
	ImGuiStyle g_Style;

	void* mem_alloc(size_t sz, void* user_data)
	{
		return mi_malloc(sz);
	}

	void mem_free(void* ptr, void* user_data)
	{
		mi_free(ptr);
	}

	namespace ui
	{
		void imgui_init(const char* ini)
		{
			auto& window = entt::locator<Window>::value();

			// clang-format off
			IMGUI_CHECKVERSION();
			ImGui::SetAllocatorFunctions(mem_alloc, mem_free, nullptr);
			ImGuiFreeType::SetAllocatorFunctions(mem_alloc, mem_free, nullptr);
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			// clang-format on

			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			io.ConfigInputTextCursorBlink = true;
			io.ConfigDockingWithShift     = true;
			// io.ConfigDockingAlwaysTabBar  = true;
			io.ConfigWindowsMoveFromTitleBarOnly = true;
			io.ConfigWindowsResizeFromEdges      = true;
			io.MouseDrawCursor                   = false;
			io.ConfigDpiScaleFonts               = true;
			io.ConfigDpiScaleViewports           = true;
			io.FontAllowUserScaling              = true;
			io.IniFilename                       = ini;
			io.ConfigColorEditFlags =
				ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar;

			ImGui_ImplSDL3_InitForOpenGL(window.handle(), window.context());
			ImGui_ImplOpenGL3_Init("#version 460 core");

			ImFontConfig font_cfg          = {};
			font_cfg.FontDataOwnedByAtlas  = false;
			font_cfg.RasterizerMultiply    = 1.5f;
			font_cfg.OversampleH           = 1;
			font_cfg.OversampleV           = 1;
			font_cfg.FontLoaderFlags      |= ImGuiFreeTypeLoaderFlags_LoadColor;
			io.FontDefault                 = io.Fonts->AddFontFromMemoryTTF(&resource::roboto_light, resource::roboto_light_len, 20.0f, &font_cfg);

			static const ImWchar icons_ranges[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0};
			ImFontConfig         md_icons_cfg;
			md_icons_cfg.FontDataOwnedByAtlas = false;
			md_icons_cfg.MergeMode            = true;
			md_icons_cfg.PixelSnapH           = true;
			io.Fonts->AddFontFromMemoryTTF(&materialdesignicons_ttf, sizeof(materialdesignicons_ttf), 20.0f, &md_icons_cfg, icons_ranges);

			// Preserve initial copy.
			g_Style = ImGui::GetStyle();

			// Then set theme.
			imgui_set_theme(window.get_display_scale());
		}

		void imgui_set_theme(const float scale) noexcept
		{
			if (ImGui::GetCurrentContext())
			{
				// Copy back over.
				ImGui::GetCurrentContext()->Style = g_Style;

				ImGuiStyle& style  = ImGui::GetStyle();
				ImVec4*     colors = style.Colors;

				style.WindowPadding     = ImVec2(10.0f, 10.0f);
				style.FramePadding      = ImVec2(6.0f, 4.0f);
				style.ItemSpacing       = ImVec2(8.0f, 6.0f);
				style.ScrollbarSize     = 14.0f;
				style.GrabMinSize       = 12.0f;
				style.WindowRounding    = 6.0f;
				style.FrameRounding     = 4.0f;
				style.PopupRounding     = 4.0f;
				style.ScrollbarRounding = 12.0f;
				style.GrabRounding      = 4.0f;
				style.TabRounding       = 4.0f;
				style.WindowBorderSize  = 1.0f;
				style.FrameBorderSize   = 1.0f;

				colors[ImGuiCol_Text]                 = ImVec4(0.97f, 0.97f, 0.95f, 1.00f);
				colors[ImGuiCol_TextDisabled]         = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
				colors[ImGuiCol_WindowBg]             = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
				colors[ImGuiCol_ChildBg]              = ImVec4(0.16f, 0.16f, 0.21f, 0.00f);
				colors[ImGuiCol_PopupBg]              = ImVec4(0.16f, 0.16f, 0.21f, 0.96f);
				colors[ImGuiCol_Border]               = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
				colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
				colors[ImGuiCol_FrameBg]              = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
				colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
				colors[ImGuiCol_FrameBgActive]        = ImVec4(0.48f, 0.55f, 0.74f, 1.00f);
				colors[ImGuiCol_TitleBg]              = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
				colors[ImGuiCol_TitleBgActive]        = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
				colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
				colors[ImGuiCol_MenuBarBg]            = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
				colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
				colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
				colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
				colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.48f, 0.55f, 0.74f, 1.00f);
				colors[ImGuiCol_CheckMark]            = ImVec4(0.31f, 0.98f, 0.48f, 1.00f);
				colors[ImGuiCol_SliderGrab]           = ImVec4(0.74f, 0.58f, 0.98f, 1.00f);
				colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.84f, 0.68f, 1.00f, 1.00f);
				colors[ImGuiCol_Button]               = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
				colors[ImGuiCol_ButtonHovered]        = ImVec4(1.00f, 0.47f, 0.78f, 1.00f);
				colors[ImGuiCol_ButtonActive]         = ImVec4(0.80f, 0.37f, 0.62f, 1.00f);
				colors[ImGuiCol_Header]               = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
				colors[ImGuiCol_HeaderHovered]        = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
				colors[ImGuiCol_HeaderActive]         = ImVec4(0.48f, 0.55f, 0.74f, 1.00f);
				colors[ImGuiCol_Tab]                  = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
				colors[ImGuiCol_TabHovered]           = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
				colors[ImGuiCol_TabSelected]          = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
				colors[ImGuiCol_TabDimmed]            = ImVec4(0.13f, 0.14f, 0.18f, 1.00f);
				colors[ImGuiCol_TabDimmedSelected]    = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);
				colors[ImGuiCol_TableHeaderBg]        = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
				colors[ImGuiCol_TableBorderStrong]    = ImVec4(0.38f, 0.45f, 0.64f, 1.00f);
				colors[ImGuiCol_TableBorderLight]     = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
				colors[ImGuiCol_PlotLines]            = ImVec4(0.55f, 0.91f, 0.99f, 1.00f);
				colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
				colors[ImGuiCol_NavCursor]            = ImVec4(0.74f, 0.58f, 0.98f, 1.00f);
				colors[ImGuiCol_DockingPreview]       = ImVec4(0.74f, 0.58f, 0.98f, 0.50f);
				colors[ImGuiCol_DockingEmptyBg]       = ImVec4(0.16f, 0.16f, 0.21f, 1.00f);

				style.FontSizeBase = 17.0f;
				style.FontScaleDpi = scale;
				style.ScaleAllSizes(scale);
			}
		}

		void imgui_handle_events(const SDL_Event& events)
		{
			if (ImGui::GetCurrentContext())
			{
				ImGui_ImplSDL3_ProcessEvent(&events);
			}
		}

		void imgui_new_frame()
		{
			if (ImGui::GetCurrentContext())
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplSDL3_NewFrame();
				ImGui::NewFrame();
			}
		}

		void imgui_render()
		{
			if (ImGui::GetCurrentContext())
			{
				sl::detail::gl2d_flush();
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
		}

		void imgui_exit()
		{
			if (ImGui::GetCurrentContext())
			{
				ImGui_ImplOpenGL3_Shutdown();
				ImGui_ImplSDL3_Shutdown();
				ImGui::DestroyContext();
			}
		}

		void imgui_notify_success(const char* msg)
		{
			ImGui::InsertNotification({ImGuiToastType::Warning, 2000, msg});
		}

		void imgui_notify_info(const char* msg)
		{
			ImGui::InsertNotification({ImGuiToastType::Info, 2000, msg});
		}

		void imgui_notify_warning(const char* msg)
		{
			ImGui::InsertNotification({ImGuiToastType::Warning, 2000, msg});
		}

		void imgui_notify_error(const char* msg)
		{
			ImGui::InsertNotification({ImGuiToastType::Error, 2000, msg});
		}

		void imgui_center_next_window()
		{
			const auto center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, {0.5f, 0.5f});
		}

		void imgui_confirm(const char* msg, std::move_only_function<void(void)>&& yes, std::move_only_function<void(void)>&& no)
		{
			ImGui::Text(msg);

			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::Button("Yes"))
			{
				if (yes)
				{
					yes();
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("No"))
			{
				if (no)
				{
					no();
				}

				ImGui::CloseCurrentPopup();
			}
		}

		ImGuiInputTextFlags imgui_default_flags() noexcept
		{
			return ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;
		}
	} // namespace ui
} // namespace galaxy

// #ifdef GALAXY_WIN_PLATFORM
// #pragma warning(pop)
// #endif

/*
		bool imgui_imagebutton(const graphics::Texture2D& texture, const ImVec2& size, const ImVec4& bg_col, const ImVec4& tint_col)
		{
			const auto id     = std::to_string(texture.id()) + std::to_string(texture.width()) + std::to_string(texture.height());
			const auto upcast = static_cast<std::uint64_t>(texture.id());

			return ImGui::ImageButton(id.c_str(), reinterpret_cast<void*>(upcast), size, {0, 1}, {1, 0}, bg_col, tint_col);
		}

		void imgui_image(const graphics::Texture2D& texture, const ImVec2& size)
		{
			const auto upcast = static_cast<std::uint64_t>(texture.id());
			ImGui::Image(reinterpret_cast<void*>(upcast), size, {0, 1}, {1, 0});
		}

		void imgui_image(const graphics::RenderTexture& texture, const ImVec2& size)
		{
			const auto upcast = static_cast<std::uint64_t>(texture.texture());
			ImGui::Image(reinterpret_cast<void*>(upcast), size, {0, 1}, {1, 0});
		}



		bool imgui_glm_vec2(const char* label, glm::vec2& vec)
		{
			auto clicked = false;

			ImGui::PushID(label);

			ImGui::TextUnformatted(label);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(150);
			clicked |= ImGui::InputFloat("X", &vec.x, 1.0f, 10.0f, "%.1f");

			ImGui::SameLine();

			ImGui::SetNextItemWidth(150);
			clicked |= ImGui::InputFloat("Y", &vec.y, 1.0f, 10.0f, "%.1f");

			ImGui::PopID();
			return clicked;
		}

		bool imgui_glm_vec3(const char* label, glm::vec3& vec)
		{
			auto clicked = false;

			ImGui::PushID(label);

			ImGui::TextUnformatted(label);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(100);
			clicked |= ImGui::InputFloat("X", &vec.x, 1.0f, 10.0f, "%.1f");

			ImGui::SameLine();

			ImGui::SetNextItemWidth(100);
			clicked |= ImGui::InputFloat("Y", &vec.y, 1.0f, 10.0f, "%.1f");

			ImGui::SameLine();

			ImGui::SetNextItemWidth(100);
			clicked |= ImGui::InputFloat("Z", &vec.z, 1.0f, 10.0f, "%.1f");

			ImGui::PopID();
			return clicked;
		}

		bool imgui_glm_vec4(const char* label, glm::vec4& vec)
		{
			auto clicked = false;

			ImGui::PushID(label);

			ImGui::TextUnformatted(label);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("X", &vec.x, 1.0f, 10.0f, "%.1f");

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("Y", &vec.y, 1.0f, 10.0f, "%.1f");

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("Z", &vec.z, 1.0f, 10.0f, "%.1f");

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("W", &vec.w, 1.0f, 10.0f, "%.1f");

			ImGui::PopID();
			return clicked;
		}

		bool imgui_frect(const char* label, math::fRect& rect)
		{
			auto clicked = false;

			ImGui::PushID(label);

			ImGui::TextUnformatted(label);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("X", &rect.x, 1.0f, 10.0f, "%.1f");

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("Y", &rect.y, 1.0f, 10.0f, "%.1f");

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("W", &rect.width, 1.0f, 10.0f, "%.1f");

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("H", &rect.height, 1.0f, 10.0f, "%.1f");

			ImGui::PopID();
			return clicked;
		}
*/
