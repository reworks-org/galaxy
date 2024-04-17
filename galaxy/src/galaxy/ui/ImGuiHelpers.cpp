///
/// ImGuiHelpers.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <ankerl/unordered_dense.h>
#include <glad/glad.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imnotify/ImGuiNotify.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/graphics/gl/Texture2D.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "ImGuiHelpers.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 4312)
#endif

namespace galaxy
{
	void* ImGuiMemAllocFunc(size_t sz, void* user_data)
	{
		return mi_malloc(sz);
	}

	void ImGuiMemFreeFunc(void* ptr, void* user_data)
	{
		mi_free(ptr);
	}

	namespace ui
	{
		ImGuiIO& imgui_init_context()
		{
			// clang-format off
			IMGUI_CHECKVERSION();
			ImGui::SetAllocatorFunctions(ImGuiMemAllocFunc, ImGuiMemFreeFunc, nullptr);
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			// clang-format on

			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			io.ConfigInputTextCursorBlink = true;
			io.ConfigDockingWithShift     = true;
			io.ConfigDockingAlwaysTabBar  = true;
			io.MouseDrawCursor            = false;

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding              = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			auto& window = core::ServiceLocator<core::Window>::ref();
			ImGui_ImplGlfw_InitForOpenGL(window.handle(), true);
			ImGui_ImplOpenGL3_Init("#version 460 core");

			ImGui::SetColorEditOptions(ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB |
									   ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar);

			return io;
		}

		void imgui_new_frame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			const ImGuiViewport* imgui_viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(imgui_viewport->WorkPos);
			ImGui::SetNextWindowSize(imgui_viewport->WorkSize);
			ImGui::SetNextWindowViewport(imgui_viewport->ID);
		}

		void imgui_render()
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// Update and Render additional Platform Windows
			// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
			// For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
			const auto& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}

		void imgui_destroy_context()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void imgui_set_theme()
		{
			// Rounded Visual Studio style by RedNicStone from ImThemes
			ImGuiStyle& style = ImGui::GetStyle();

			style.Alpha                     = 1.0f;
			style.DisabledAlpha             = 0.6000000238418579f;
			style.WindowPadding             = ImVec2(8.0f, 8.0f);
			style.WindowRounding            = 4.0f;
			style.WindowBorderSize          = 0.0f;
			style.WindowMinSize             = ImVec2(32.0f, 32.0f);
			style.WindowTitleAlign          = ImVec2(0.0f, 0.5f);
			style.WindowMenuButtonPosition  = ImGuiDir_Left;
			style.ChildRounding             = 0.0f;
			style.ChildBorderSize           = 1.0f;
			style.PopupRounding             = 4.0f;
			style.PopupBorderSize           = 1.0f;
			style.FramePadding              = ImVec2(4.0f, 3.0f);
			style.FrameRounding             = 2.5f;
			style.FrameBorderSize           = 0.0f;
			style.ItemSpacing               = ImVec2(8.0f, 4.0f);
			style.ItemInnerSpacing          = ImVec2(4.0f, 4.0f);
			style.CellPadding               = ImVec2(4.0f, 2.0f);
			style.IndentSpacing             = 21.0f;
			style.ColumnsMinSpacing         = 6.0f;
			style.ScrollbarSize             = 11.0f;
			style.ScrollbarRounding         = 2.5f;
			style.GrabMinSize               = 10.0f;
			style.GrabRounding              = 2.0f;
			style.TabRounding               = 3.5f;
			style.TabBorderSize             = 0.0f;
			style.TabMinWidthForCloseButton = 0.0f;
			style.ColorButtonPosition       = ImGuiDir_Right;
			style.ButtonTextAlign           = ImVec2(0.5f, 0.5f);
			style.SelectableTextAlign       = ImVec2(0.0f, 0.0f);

			style.Colors[ImGuiCol_Text]                  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.5921568870544434f, 0.5921568870544434f, 0.5921568870544434f, 1.0f);
			style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
			style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
			style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
			style.Colors[ImGuiCol_Border]                = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
			style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
			style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
			style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
			style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
			style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
			style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
			style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
			style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
			style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
			style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
			style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
			style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
			style.Colors[ImGuiCol_Button]                = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
			style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
			style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
			style.Colors[ImGuiCol_Header]                = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
			style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
			style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
			style.Colors[ImGuiCol_Separator]             = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
			style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
			style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
			style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
			style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
			style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
			style.Colors[ImGuiCol_Tab]                   = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
			style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
			style.Colors[ImGuiCol_TabActive]             = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
			style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
			style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
			style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
			style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
			style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
			style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
			style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
			style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
			style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
			style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
			style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
			style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
			style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
			style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
			style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
		}

		void imgui_popup(const char* id, bool& open, const std::function<void(void)>& func)
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

		void imgui_confirm(const char* msg, const std::function<void(void)>& yes, const std::function<void(void)>& no)
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

		void imgui_center_next_window()
		{
			const auto center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, {0.5f, 0.5f});
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
			clicked |= ImGui::InputFloat("X", &vec.x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(150);
			clicked |= ImGui::InputFloat("Y", &vec.y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

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
			clicked |= ImGui::InputFloat("X", &vec.x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(100);
			clicked |= ImGui::InputFloat("Y", &vec.y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(100);
			clicked |= ImGui::InputFloat("Z", &vec.z, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

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
			clicked |= ImGui::InputFloat("X", &vec.x, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("Y", &vec.y, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("Z", &vec.z, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::SameLine();

			ImGui::SetNextItemWidth(75);
			clicked |= ImGui::InputFloat("W", &vec.w, 1.0f, 10.0f, "%.1f", ImGuiInputTextFlags_CharsNoBlank);

			ImGui::PopID();
			return clicked;
		}
	} // namespace ui
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
