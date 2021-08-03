///
/// MainMenu.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileListener.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "../layers/Editor.hpp"
#include "../resources/Roboto-Light.hpp"

#include "MainMenu.hpp"

using namespace galaxy;

namespace sc
{
	MainMenu::MainMenu() noexcept
		: m_editor {nullptr}
	{
		set_name("Main Menu");

		ImGuiIO& io = ImGui::GetIO();

		ImFontConfig font_config         = {};
		font_config.FontDataOwnedByAtlas = false;
		font_config.RasterizerMultiply   = 1.5f;
		font_config.OversampleH          = 4;
		m_bigger_default_font = io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(&ttf::roboto_light), ttf::roboto_light_len, 144.0f, &font_config);

		//m_scene_stack.create("MainMenu");
		//m_scene_stack.push("MainMenu");
	}

	MainMenu::~MainMenu() noexcept
	{
		m_editor = nullptr;

		//m_scene_stack.clear();
	}

	void MainMenu::events()
	{
		//m_scene_stack.events();
	}

	void MainMenu::update()
	{
		//m_scene_stack.update();
	}

	void MainMenu::pre_render()
	{
		ui::imgui_new_frame();

		// clang-format off
		static constexpr const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		// clang-format on

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		ImGui::Begin("Main Menu", NULL, window_flags);
		ImGui::PopStyleVar(3);

		const float window_calc             = ImGui::GetWindowWidth() / 2.0f;
		constexpr const float button_width  = 200.0f;
		constexpr const float button_height = 40.0f;

		ImGui::PushFont(m_bigger_default_font);
		ImGui::SetCursorPos({window_calc - (ImGui::CalcTextSize("Supercluster Editor").x / 2.0f), 100.0f});
		ImGui::Text("Supercluster Editor");
		ImGui::PopFont();

		ImGui::SetCursorPos({window_calc - (button_width / 2.0f), (ImGui::GetWindowHeight() / 2.0f) - ((button_height * 3) / 2.0f)});

		if (ImGui::Button("New", {button_width, button_height}))
		{
			m_editor->new_project();
			m_app->push_layer(m_editor);
		}

		ImGui::Spacing();
		ImGui::SetCursorPosX(window_calc - (button_width / 2.0f));

		if (ImGui::Button("Load", {button_width, button_height}))
		{
			auto file = SL_HANDLE.vfs()->show_open_dialog("*.scproj", "projects/");
			if (file != std::nullopt)
			{
				m_editor->load_project(file.value());
				m_app->push_layer(m_editor);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to open selected project file.");
			}
		}

		ImGui::Spacing();
		ImGui::SetCursorPosX(window_calc - (button_width / 2.0f));

		if (ImGui::Button("Exit", {button_width, button_height}))
		{
			SL_HANDLE.window()->close();
		}

		ImGui::End();

		//m_scene_stack.pre_render();
	}

	void MainMenu::render()
	{
		ui::imgui_render();

		//m_scene_stack.render();
	}
} // namespace sc