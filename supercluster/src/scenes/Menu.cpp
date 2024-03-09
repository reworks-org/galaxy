///
/// Menu.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/scene/Scene.hpp>

#include "Editor.hpp"

#include "Menu.hpp"

namespace sc
{
	Menu::Menu(const std::string& name)
		: Scene(name)
		, m_load {false}
	{
		if (m_bg.load("sc.png"))
		{
			m_bg.filter(graphics::TextureFilter::TRILINEAR);
		}
	}

	Menu::~Menu()
	{
	}

	void Menu::load()
	{
		core::ServiceLocator<core::Window>::ref().restore();
	}

	void Menu::unload()
	{
	}

	void Menu::update()
	{
		if (m_load)
		{
			m_load = false;
			load_project();
		}
	}

	void Menu::render()
	{
		ui::imgui_new_frame();

		static constexpr const auto window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
												   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		ImGui::Begin("Main Menu", NULL, window_flags);
		ImGui::PopStyleVar(3);

		ui::imgui_image(m_bg, {ImGui::GetWindowWidth(), ImGui::GetWindowHeight()});

		const auto           window_calc   = ImGui::GetWindowWidth() / 2.0f;
		constexpr const auto button_width  = 200.0f;
		constexpr const auto button_height = 40.0f;

		ImGui::SetCursorPos({window_calc - (button_width / 2.0f), (ImGui::GetWindowHeight() / 2.0f) - ((button_height * 3) / 2.0f)});

		if (ImGui::Button("New", {button_width, button_height}))
		{
			auto& sm = core::ServiceLocator<scene::SceneManager>::ref();
			if (auto ptr = sm.get("sc_editor"))
			{
				auto editor = std::static_pointer_cast<sc::Editor>(ptr);
				editor->new_project();

				ptr->m_enabled  = true;
				this->m_enabled = false;
			}
		}

		ImGui::Spacing();
		ImGui::SetCursorPosX(window_calc - (button_width / 2.0f));

		if (ImGui::Button("Load", {button_width, button_height}))
		{
			m_load = true;
		}

		ImGui::Spacing();
		ImGui::SetCursorPosX(window_calc - (button_width / 2.0f));

		if (ImGui::Button("Exit", {button_width, button_height}))
		{
			core::ServiceLocator<core::Window>::ref().close();
		}

		ImGui::End();

		ui::imgui_render();
	}

	void Menu::load_project()
	{
		auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

		const auto path = fs.open_file_dialog({"*.scproj"}, "../editor_data/projects");
		if (!path.empty())
		{
			auto& sm = core::ServiceLocator<scene::SceneManager>::ref();
			if (auto ptr = sm.get("sc_editor"))
			{
				auto editor = std::static_pointer_cast<sc::Editor>(ptr);
				editor->load_project(path);

				ptr->m_enabled  = true;
				this->m_enabled = false;
			}
		}
	}
} // namespace sc
