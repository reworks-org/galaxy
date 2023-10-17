///
/// Theme.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <cstdint>

#include <imgui.h>

#include "ImGuiTheme.hpp"

namespace galaxy
{
	namespace ui
	{
		void imgui_theme_dracula()
		{
			auto& colors               = ImGui::GetStyle().Colors;
			colors[ImGuiCol_WindowBg]  = ImVec4 {0.1f, 0.1f, 0.13f, 1.0f};
			colors[ImGuiCol_MenuBarBg] = ImVec4 {0.16f, 0.16f, 0.21f, 1.0f};

			// Border
			colors[ImGuiCol_Border]       = ImVec4 {0.44f, 0.37f, 0.61f, 0.29f};
			colors[ImGuiCol_BorderShadow] = ImVec4 {0.0f, 0.0f, 0.0f, 0.24f};

			// Text
			colors[ImGuiCol_Text]         = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4 {0.5f, 0.5f, 0.5f, 1.0f};

			// Headers
			colors[ImGuiCol_Header]        = ImVec4 {0.13f, 0.13f, 0.17, 1.0f};
			colors[ImGuiCol_HeaderHovered] = ImVec4 {0.19f, 0.2f, 0.25f, 1.0f};
			colors[ImGuiCol_HeaderActive]  = ImVec4 {0.16f, 0.16f, 0.21f, 1.0f};

			// Buttons
			colors[ImGuiCol_Button]        = ImVec4 {0.13f, 0.13f, 0.17, 1.0f};
			colors[ImGuiCol_ButtonHovered] = ImVec4 {0.19f, 0.2f, 0.25f, 1.0f};
			colors[ImGuiCol_ButtonActive]  = ImVec4 {0.16f, 0.16f, 0.21f, 1.0f};
			colors[ImGuiCol_CheckMark]     = ImVec4 {0.74f, 0.58f, 0.98f, 1.0f};

			// Popups
			colors[ImGuiCol_PopupBg] = ImVec4 {0.1f, 0.1f, 0.13f, 0.92f};

			// Slider
			colors[ImGuiCol_SliderGrab]       = ImVec4 {0.44f, 0.37f, 0.61f, 0.54f};
			colors[ImGuiCol_SliderGrabActive] = ImVec4 {0.74f, 0.58f, 0.98f, 0.54f};

			// Frame BG
			colors[ImGuiCol_FrameBg]        = ImVec4 {0.13f, 0.13, 0.17, 1.0f};
			colors[ImGuiCol_FrameBgHovered] = ImVec4 {0.19f, 0.2f, 0.25f, 1.0f};
			colors[ImGuiCol_FrameBgActive]  = ImVec4 {0.16f, 0.16f, 0.21f, 1.0f};

			// Tabs
			colors[ImGuiCol_Tab]                = ImVec4 {0.16f, 0.16f, 0.21f, 1.0f};
			colors[ImGuiCol_TabHovered]         = ImVec4 {0.24, 0.24f, 0.32f, 1.0f};
			colors[ImGuiCol_TabActive]          = ImVec4 {0.2f, 0.22f, 0.27f, 1.0f};
			colors[ImGuiCol_TabUnfocused]       = ImVec4 {0.16f, 0.16f, 0.21f, 1.0f};
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4 {0.16f, 0.16f, 0.21f, 1.0f};

			// Title
			colors[ImGuiCol_TitleBg]          = ImVec4 {0.16f, 0.16f, 0.21f, 1.0f};
			colors[ImGuiCol_TitleBgActive]    = ImVec4 {0.16f, 0.16f, 0.21f, 1.0f};
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4 {0.16f, 0.16f, 0.21f, 1.0f};

			// Scrollbar
			colors[ImGuiCol_ScrollbarBg]          = ImVec4 {0.1f, 0.1f, 0.13f, 1.0f};
			colors[ImGuiCol_ScrollbarGrab]        = ImVec4 {0.16f, 0.16f, 0.21f, 1.0f};
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4 {0.19f, 0.2f, 0.25f, 1.0f};
			colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4 {0.24f, 0.24f, 0.32f, 1.0f};

			// Seperator
			colors[ImGuiCol_Separator]        = ImVec4 {0.44f, 0.37f, 0.61f, 1.0f};
			colors[ImGuiCol_SeparatorHovered] = ImVec4 {0.74f, 0.58f, 0.98f, 1.0f};
			colors[ImGuiCol_SeparatorActive]  = ImVec4 {0.84f, 0.58f, 1.0f, 1.0f};

			// Resize Grip
			colors[ImGuiCol_ResizeGrip]        = ImVec4 {0.44f, 0.37f, 0.61f, 0.29f};
			colors[ImGuiCol_ResizeGripHovered] = ImVec4 {0.74f, 0.58f, 0.98f, 0.29f};
			colors[ImGuiCol_ResizeGripActive]  = ImVec4 {0.84f, 0.58f, 1.0f, 0.29f};

			// Docking
			colors[ImGuiCol_DockingPreview] = ImVec4 {0.44f, 0.37f, 0.61f, 1.0f};

			auto& style             = ImGui::GetStyle();
			style.TabRounding       = 4;
			style.ScrollbarRounding = 9;
			style.WindowRounding    = 7;
			style.GrabRounding      = 3;
			style.FrameRounding     = 3;
			style.PopupRounding     = 4;
			style.ChildRounding     = 4;
		}

		void imgui_theme_material_dark()
		{
			ImGui::GetStyle().FrameRounding = 4.0f;
			ImGui::GetStyle().GrabRounding  = 4.0f;

			ImVec4* colors                         = ImGui::GetStyle().Colors;
			colors[ImGuiCol_Text]                  = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
			colors[ImGuiCol_TextDisabled]          = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
			colors[ImGuiCol_WindowBg]              = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_ChildBg]               = ImVec4(0.15f, 0.18f, 0.22f, 0.00f);
			colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border]                = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
			colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
			colors[ImGuiCol_FrameBgActive]         = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
			colors[ImGuiCol_TitleBg]               = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
			colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			colors[ImGuiCol_MenuBarBg]             = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
			colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
			colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
			colors[ImGuiCol_CheckMark]             = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_SliderGrab]            = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
			colors[ImGuiCol_Button]                = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_ButtonHovered]         = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_ButtonActive]          = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
			colors[ImGuiCol_Header]                = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
			colors[ImGuiCol_HeaderHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
			colors[ImGuiCol_HeaderActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_Separator]             = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
			colors[ImGuiCol_SeparatorActive]       = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
			colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
			colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
			colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			colors[ImGuiCol_Tab]                   = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_TabHovered]            = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
			colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_TabUnfocused]          = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

			colors[ImGuiCol_DockingPreview] = colors[ImGuiCol_WindowBg];

			ImGui::GetStyle().FrameBorderSize  = 1.0f;
			ImGui::GetStyle().WindowBorderSize = 1.0f;
		}

		void imgui_theme_visual_dark()
		{
			ImGuiStyle& style = ImGui::GetStyle();

			style.Alpha                     = 1.0;
			style.DisabledAlpha             = 0.6000000238418579;
			style.WindowPadding             = ImVec2(8.0, 8.0);
			style.WindowRounding            = 0.0;
			style.WindowBorderSize          = 1.0;
			style.WindowMinSize             = ImVec2(32.0, 32.0);
			style.WindowTitleAlign          = ImVec2(0.0, 0.5);
			style.WindowMenuButtonPosition  = ImGuiDir_Left;
			style.ChildRounding             = 0.0;
			style.ChildBorderSize           = 1.0;
			style.PopupRounding             = 0.0;
			style.PopupBorderSize           = 1.0;
			style.FramePadding              = ImVec2(4.0, 3.0);
			style.FrameRounding             = 0.0;
			style.FrameBorderSize           = 0.0;
			style.ItemSpacing               = ImVec2(8.0, 4.0);
			style.ItemInnerSpacing          = ImVec2(4.0, 4.0);
			style.CellPadding               = ImVec2(4.0, 2.0);
			style.IndentSpacing             = 21.0;
			style.ColumnsMinSpacing         = 6.0;
			style.ScrollbarSize             = 14.0;
			style.ScrollbarRounding         = 0.0;
			style.GrabMinSize               = 10.0;
			style.GrabRounding              = 0.0;
			style.TabRounding               = 0.0;
			style.TabBorderSize             = 0.0;
			style.TabMinWidthForCloseButton = 0.0;
			style.ColorButtonPosition       = ImGuiDir_Right;
			style.ButtonTextAlign           = ImVec2(0.5, 0.5);
			style.SelectableTextAlign       = ImVec2(0.0, 0.0);

			style.Colors[ImGuiCol_Text]                  = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.5921568870544434, 0.5921568870544434, 0.5921568870544434, 1.0);
			style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
			style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
			style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
			style.Colors[ImGuiCol_Border]                = ImVec4(0.3058823645114899, 0.3058823645114899, 0.3058823645114899, 1.0);
			style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.3058823645114899, 0.3058823645114899, 0.3058823645114899, 1.0);
			style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2156862765550613, 1.0);
			style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
			style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
			style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
			style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
			style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
			style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2156862765550613, 1.0);
			style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2156862765550613, 1.0);
			style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.321568638086319, 0.321568638086319, 0.3333333432674408, 1.0);
			style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.3529411852359772, 0.3529411852359772, 0.3725490272045135, 1.0);
			style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.3529411852359772, 0.3529411852359772, 0.3725490272045135, 1.0);
			style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
			style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
			style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
			style.Colors[ImGuiCol_Button]                = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2156862765550613, 1.0);
			style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
			style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
			style.Colors[ImGuiCol_Header]                = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2156862765550613, 1.0);
			style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
			style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
			style.Colors[ImGuiCol_Separator]             = ImVec4(0.3058823645114899, 0.3058823645114899, 0.3058823645114899, 1.0);
			style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.3058823645114899, 0.3058823645114899, 0.3058823645114899, 1.0);
			style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.3058823645114899, 0.3058823645114899, 0.3058823645114899, 1.0);
			style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
			style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.2000000029802322, 0.2000000029802322, 0.2156862765550613, 1.0);
			style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.321568638086319, 0.321568638086319, 0.3333333432674408, 1.0);
			style.Colors[ImGuiCol_Tab]                   = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
			style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
			style.Colors[ImGuiCol_TabActive]             = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
			style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
			style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
			style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
			style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
			style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
			style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.1137254908680916, 0.5921568870544434, 0.9254902005195618, 1.0);
			style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.1882352977991104, 0.1882352977991104, 0.2000000029802322, 1.0);
			style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.3098039329051971, 0.3098039329051971, 0.3490196168422699, 1.0);
			style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.2274509817361832, 0.2274509817361832, 0.2470588237047195, 1.0);
			style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0.0, 0.0, 0.0, 0.0);
			style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0, 1.0, 1.0, 0.05999999865889549);
			style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.0, 0.4666666686534882, 0.7843137383460999, 1.0);
			style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
			style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0, 1.0, 1.0, 0.699999988079071);
			style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.800000011920929, 0.800000011920929, 0.800000011920929, 0.2000000029802322);
			style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.1450980454683304, 0.1450980454683304, 0.1490196138620377, 1.0);
		}

		void imgui_theme_fancy_dark()
		{
			ImGuiStyle& style                            = ImGui::GetStyle();
			style.Colors[ImGuiCol_Text]                  = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
			style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
			style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
			style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
			style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
			style.Colors[ImGuiCol_Button]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
			style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_Header]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
			style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
			style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
			style.Colors[ImGuiCol_Separator]             = style.Colors[ImGuiCol_Border];
			style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
			style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
			style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			style.Colors[ImGuiCol_Tab]                   = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
			style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
			style.Colors[ImGuiCol_TabActive]             = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
			style.Colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
			style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
			style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
			style.GrabRounding = style.FrameRounding = 2.3f;
		}

		void imgui_theme_dark_embrace()
		{
			ImVec4* colors                         = ImGui::GetStyle().Colors;
			colors[ImGuiCol_Text]                  = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
			colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
			colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
			colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_PopupBg]               = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
			colors[ImGuiCol_Border]                = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
			colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
			colors[ImGuiCol_FrameBg]               = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
			colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
			colors[ImGuiCol_FrameBgActive]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
			colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_TitleBgActive]         = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
			colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
			colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
			colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
			colors[ImGuiCol_CheckMark]             = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
			colors[ImGuiCol_SliderGrab]            = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
			colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
			colors[ImGuiCol_Button]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
			colors[ImGuiCol_ButtonHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
			colors[ImGuiCol_ButtonActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
			colors[ImGuiCol_Header]                = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
			colors[ImGuiCol_HeaderHovered]         = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
			colors[ImGuiCol_HeaderActive]          = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
			colors[ImGuiCol_Separator]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
			colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
			colors[ImGuiCol_SeparatorActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
			colors[ImGuiCol_ResizeGrip]            = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
			colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
			colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
			colors[ImGuiCol_Tab]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
			colors[ImGuiCol_TabHovered]            = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
			colors[ImGuiCol_TabUnfocused]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
			colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_DockingPreview]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
			colors[ImGuiCol_DockingEmptyBg]        = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogram]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
			colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
			colors[ImGuiCol_TableBorderLight]      = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
			colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
			colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
			colors[ImGuiCol_DragDropTarget]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
			colors[ImGuiCol_NavHighlight]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

			ImGuiStyle& style       = ImGui::GetStyle();
			style.WindowPadding     = ImVec2(8.00f, 8.00f);
			style.FramePadding      = ImVec2(5.00f, 2.00f);
			style.CellPadding       = ImVec2(6.00f, 6.00f);
			style.ItemSpacing       = ImVec2(6.00f, 6.00f);
			style.ItemInnerSpacing  = ImVec2(6.00f, 6.00f);
			style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
			style.IndentSpacing     = 25;
			style.ScrollbarSize     = 15;
			style.GrabMinSize       = 10;
			style.WindowBorderSize  = 1;
			style.ChildBorderSize   = 1;
			style.PopupBorderSize   = 1;
			style.FrameBorderSize   = 1;
			style.TabBorderSize     = 1;
			style.WindowRounding    = 7;
			style.ChildRounding     = 4;
			style.FrameRounding     = 3;
			style.PopupRounding     = 4;
			style.ScrollbarRounding = 9;
			style.GrabRounding      = 3;
			style.LogSliderDeadzone = 4;
			style.TabRounding       = 4;
		}

		void imgui_theme_enhanced_dark()
		{
			ImVec4* colors = ImGui::GetStyle().Colors;

			colors[ImGuiCol_Text]                  = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
			colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
			colors[ImGuiCol_WindowBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_ChildBg]               = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_PopupBg]               = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_Border]                = ImVec4(0.275f, 0.275f, 0.275f, 1.00f);
			colors[ImGuiCol_BorderShadow]          = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
			colors[ImGuiCol_FrameBg]               = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
			colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
			colors[ImGuiCol_FrameBgActive]         = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
			colors[ImGuiCol_TitleBg]               = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
			colors[ImGuiCol_TitleBgActive]         = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
			colors[ImGuiCol_MenuBarBg]             = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
			colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
			colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
			colors[ImGuiCol_CheckMark]             = ImVec4(0.00f, 0.44f, 0.88f, 1.00f);
			colors[ImGuiCol_SliderGrab]            = ImVec4(0.000f, 0.434f, 0.878f, 1.000f);
			colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.000f, 0.434f, 0.878f, 1.000f);
			colors[ImGuiCol_Button]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
			colors[ImGuiCol_ButtonHovered]         = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
			colors[ImGuiCol_ButtonActive]          = ImVec4(0.000f, 0.439f, 0.878f, 0.824f);
			colors[ImGuiCol_Header]                = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
			colors[ImGuiCol_HeaderHovered]         = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
			colors[ImGuiCol_HeaderActive]          = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
			colors[ImGuiCol_Separator]             = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
			colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
			colors[ImGuiCol_SeparatorActive]       = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
			colors[ImGuiCol_ResizeGrip]            = ImVec4(0.082f, 0.082f, 0.082f, 1.00f);
			colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
			colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			colors[ImGuiCol_Tab]                   = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
			colors[ImGuiCol_TabHovered]            = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
			colors[ImGuiCol_TabActive]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_TabUnfocused]          = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
			colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_DockingPreview]        = ImVec4(0.19f, 0.53f, 0.78f, 0.22f);
			colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotLines]             = ImVec4(0.00f, 0.44f, 0.88f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.00f, 0.47f, 0.94f, 1.00f);
			colors[ImGuiCol_PlotHistogram]         = ImVec4(0.00f, 0.44f, 0.88f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.00f, 0.47f, 0.94f, 1.00f);
			colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
			colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.197f, 0.197f, 0.197f, 1.00f);
			colors[ImGuiCol_TableBorderLight]      = ImVec4(0.197f, 0.197f, 0.197f, 1.00f);
			colors[ImGuiCol_TableRowBg]            = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
			colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
			colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.188f, 0.529f, 0.780f, 1.000f);
			colors[ImGuiCol_DragDropTarget]        = ImVec4(0.00f, 0.44f, 0.88f, 1.00f);
			colors[ImGuiCol_NavHighlight]          = ImVec4(0.00f, 0.44f, 0.88f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

			ImGuiStyle* style = &ImGui::GetStyle();

			style->AntiAliasedFill        = true;
			style->AntiAliasedLines       = true;
			style->AntiAliasedLinesUseTex = true;

			style->WindowPadding             = ImVec2(4.0f, 4.0f);
			style->FramePadding              = ImVec2(4.0f, 4.0f);
			style->TabMinWidthForCloseButton = 0.1f;
			style->CellPadding               = ImVec2(8.0f, 4.0f);
			style->ItemSpacing               = ImVec2(8.0f, 3.0f);
			style->ItemInnerSpacing          = ImVec2(2.0f, 4.0f);
			style->TouchExtraPadding         = ImVec2(0.0f, 0.0f);
			style->IndentSpacing             = 12;
			style->ScrollbarSize             = 14;
			style->GrabMinSize               = 10;

			style->WindowBorderSize = 1.0f;
			style->ChildBorderSize  = 0.0f;
			style->PopupBorderSize  = 1.5f;
			style->FrameBorderSize  = 0.5f;
			style->TabBorderSize    = 0.0f;

			style->WindowRounding    = 6.0f;
			style->ChildRounding     = 0.0f;
			style->FrameRounding     = 2.0f;
			style->PopupRounding     = 2.0f;
			style->ScrollbarRounding = 3.0f;
			style->GrabRounding      = 2.0f;
			style->LogSliderDeadzone = 4.0f;
			style->TabRounding       = 3.0f;

			style->WindowTitleAlign         = ImVec2(0.0f, 0.5f);
			style->WindowMenuButtonPosition = ImGuiDir_None;
			style->ColorButtonPosition      = ImGuiDir_Left;
			style->ButtonTextAlign          = ImVec2(0.5f, 0.5f);
			style->SelectableTextAlign      = ImVec2(0.0f, 0.0f);
			style->DisplaySafeAreaPadding   = ImVec2(8.0f, 8.0f);

			ImGui::SetColorEditOptions(ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_DisplayRGB |
									   ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_Uint8);
		}
	} // namespace ui
} // namespace galaxy
