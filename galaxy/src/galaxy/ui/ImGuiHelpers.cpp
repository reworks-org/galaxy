///
/// ImGuiHelpers.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include <entt/locator/locator.hpp>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/misc/freetype/imgui_freetype.h>
#include <imgui/extensions/notify/material_design_icons.h>
#include <imgui/extensions/notify/material_design_icons_ttf.h>
#include <mimalloc.h>

#include "galaxy/core/Window.hpp"
#include "galaxy/platform/Pragma.hpp"
#include "galaxy/resource/embedded/RobotoLight.hpp"

#include "ImGuiHelpers.hpp"

// #ifdef GALAXY_WIN_PLATFORM
// #pragma warning(push)
//  #pragma warning(disable : 4312)
// #endif

namespace galaxy
{
	void* mem_alloc(size_t sz, void* user_data)
	{
		return mi_malloc(sz);
	}

	void mem_free(void* ptr, void* user_data)
	{
		mi_free(ptr);
	}

	void load_backend() noexcept
	{
		auto& window = entt::locator<Window>::value();
		ImGui_ImplSDL3_InitForOpenGL(window.handle(), window.context());
		ImGui_ImplOpenGL3_Init("#version 460 core");
	}

	void load_theme() noexcept
	{
		// Hazy Dark style by kaitabuchi314 from ImThemes.
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha                            = 1.0f;
		style.DisabledAlpha                    = 0.6f;
		style.WindowPadding                    = ImVec2(5.5f, 8.3f);
		style.WindowRounding                   = 4.5f;
		style.WindowBorderSize                 = 1.0f;
		style.WindowMinSize                    = ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign                 = ImVec2(0.0f, 0.5f);
		style.WindowMenuButtonPosition         = ImGuiDir_Left;
		style.ChildRounding                    = 3.2f;
		style.ChildBorderSize                  = 1.0f;
		style.PopupRounding                    = 2.7f;
		style.PopupBorderSize                  = 1.0f;
		style.FramePadding                     = ImVec2(4.0f, 3.0f);
		style.FrameRounding                    = 2.4f;
		style.FrameBorderSize                  = 0.0f;
		style.ItemSpacing                      = ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing                 = ImVec2(4.0f, 4.0f);
		style.CellPadding                      = ImVec2(4.0f, 2.0f);
		style.IndentSpacing                    = 21.0f;
		style.ColumnsMinSpacing                = 6.0f;
		style.ScrollbarSize                    = 14.0f;
		style.ScrollbarRounding                = 9.0f;
		style.GrabMinSize                      = 10.0f;
		style.GrabRounding                     = 3.2f;
		style.TabRounding                      = 3.5f;
		style.TabBorderSize                    = 1.0f;
		style.TabCloseButtonMinWidthUnselected = 0.0f;
		style.ColorButtonPosition              = ImGuiDir_Right;
		style.ButtonTextAlign                  = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign              = ImVec2(0.0f, 0.0f);

		style.Colors[ImGuiCol_Text]                  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.49803922f, 0.49803922f, 0.49803922f, 1.0f);
		style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.05882353f, 0.05882353f, 0.05882353f, 0.94f);
		style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.078431375f, 0.078431375f, 0.078431375f, 0.94f);
		style.Colors[ImGuiCol_Border]                = ImVec4(0.42745098f, 0.42745098f, 0.49803922f, 0.5f);
		style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.13725491f, 0.17254902f, 0.22745098f, 0.54f);
		style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.21176471f, 0.25490198f, 0.3019608f, 0.4f);
		style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.043137256f, 0.047058824f, 0.047058824f, 0.67f);
		style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.039215688f, 0.039215688f, 0.039215688f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.078431375f, 0.08235294f, 0.09019608f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
		style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.13725491f, 0.13725491f, 0.13725491f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.019607844f, 0.019607844f, 0.019607844f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.30980393f, 0.30980393f, 0.30980393f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40784314f, 0.40784314f, 0.40784314f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.50980395f, 0.50980395f, 0.50980395f, 1.0f);
		style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.7176471f, 0.78431374f, 0.84313726f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.47843137f, 0.5254902f, 0.57254905f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.2901961f, 0.31764707f, 0.3529412f, 1.0f);
		style.Colors[ImGuiCol_Button]                = ImVec4(0.14901961f, 0.16078432f, 0.1764706f, 0.4f);
		style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.13725491f, 0.14509805f, 0.15686275f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.078431375f, 0.08627451f, 0.09019608f, 1.0f);
		style.Colors[ImGuiCol_Header]                = ImVec4(0.19607843f, 0.21568628f, 0.23921569f, 0.31f);
		style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.16470589f, 0.1764706f, 0.19215687f, 0.8f);
		style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.07450981f, 0.08235294f, 0.09019608f, 1.0f);
		style.Colors[ImGuiCol_Separator]             = ImVec4(0.42745098f, 0.42745098f, 0.49803922f, 0.5f);
		style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.23921569f, 0.3254902f, 0.42352942f, 0.78f);
		style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.27450982f, 0.38039216f, 0.49803922f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.2901961f, 0.32941177f, 0.3764706f, 0.2f);
		style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.23921569f, 0.29803923f, 0.36862746f, 0.67f);
		style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.16470589f, 0.1764706f, 0.1882353f, 0.95f);
		style.Colors[ImGuiCol_Tab]                   = ImVec4(0.11764706f, 0.1254902f, 0.13333334f, 0.862f);
		style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.32941177f, 0.40784314f, 0.5019608f, 0.8f);
		style.Colors[ImGuiCol_TabSelected]           = ImVec4(0.24313726f, 0.24705882f, 0.25490198f, 1.0f);
		style.Colors[ImGuiCol_TabDimmed]             = ImVec4(0.06666667f, 0.101960786f, 0.14509805f, 0.9724f);
		style.Colors[ImGuiCol_TabDimmedSelected]     = ImVec4(0.13333334f, 0.25882354f, 0.42352942f, 1.0f);
		style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.60784316f, 0.60784316f, 0.60784316f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.0f, 0.42745098f, 0.34901962f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.8980392f, 0.69803923f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
		style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.35f);
		style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
		style.Colors[ImGuiCol_NavCursor]             = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
		style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
		style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
	}

	void load_font() noexcept
	{
		auto& io     = ImGui::GetIO();
		auto& window = entt::locator<Window>::value();

		const auto font_size = window.get_display_scale() * 16.0f;

		ImFontConfig font_cfg          = {};
		font_cfg.FontDataOwnedByAtlas  = false;
		font_cfg.RasterizerMultiply    = 1.5f;
		font_cfg.OversampleH           = 1;
		font_cfg.OversampleV           = 1;
		font_cfg.FontLoaderFlags      |= ImGuiFreeTypeLoaderFlags_LoadColor;
		io.FontDefault                 = io.Fonts->AddFontFromMemoryTTF(&resource::roboto_light, resource::roboto_light_len, font_size, &font_cfg);

		static const ImWchar icons_ranges[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0};
		ImFontConfig         md_icons_cfg;
		md_icons_cfg.FontDataOwnedByAtlas = false;
		md_icons_cfg.MergeMode            = true;
		md_icons_cfg.PixelSnapH           = true;
		io.Fonts->AddFontFromMemoryTTF(&materialdesignicons_ttf, sizeof(materialdesignicons_ttf), font_size, &md_icons_cfg, icons_ranges);
	}

	namespace ui
	{
		void imgui_init(const char* ini)
		{
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
			io.IniFilename                       = ini;

			ImGui::SetColorEditOptions(
				ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar
			);

			load_backend();
			load_theme();
			load_font();

			ui::imgui_rescale();
		}

		void imgui_new_frame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL3_NewFrame();
			ImGui::NewFrame();
		}

		void imgui_render()
		{
			ImGui::Render();

			auto& w = entt::locator<Window>::value();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glActiveTexture(GL_TEXTURE0);
			glViewport(0, 0, w.get_pixel_size().x, w.get_pixel_size().y);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void imgui_exit()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplSDL3_Shutdown();
			ImGui::DestroyContext();
		}

		void imgui_rescale()
		{
			auto& style = ImGui::GetStyle();

			const auto& io     = ImGui::GetIO();
			const auto& window = entt::locator<Window>::value();
			style.ScaleAllSizes(window.get_display_scale());
			style.FontScaleDpi = window.get_display_scale();
		}
	} // namespace ui
} // namespace galaxy

// #ifdef GALAXY_WIN_PLATFORM
// #pragma warning(pop)
// #endif

/*
		void imgui_center_next_window()
		{
			const auto center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, {0.5f, 0.5f});
		}

				bool imgui_loaded()
		{
			return ImGui::GetCurrentContext() != nullptr;
		}

		void imgui_popup(const char* id, bool& open, std::move_only_function<void(void)>&& func)
		{
			if (open)
			{
				ImGui::OpenPopup(id);
				open = false;
			}

			imgui_center_next_window();
			if (ImGui::BeginPopup(id))
			{
				func();
				ImGui::EndPopup();
			}
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
