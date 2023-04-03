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
	Menu::Menu(std::string_view name, scene::Scene* scene)
		: Layer {name, scene}
		, m_load {false}
	{
		m_bg.load("../editor_data/sc.png");
		m_bg.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
		m_bg.set_filter(graphics::TextureFilters::MAG_TRILINEAR);
	}

	Menu::~Menu()
	{
	}

	void Menu::on_push()
	{
		core::ServiceLocator<core::Window>::ref().restore();
	}

	void Menu::on_pop()
	{
	}

	void Menu::events()
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

		const auto window_calc             = ImGui::GetWindowWidth() / 2.0f;
		constexpr const auto button_width  = 200.0f;
		constexpr const auto button_height = 40.0f;

		ImGui::SetCursorPos({window_calc - (button_width / 2.0f), (ImGui::GetWindowHeight() / 2.0f) - ((button_height * 3) / 2.0f)});

		if (ImGui::Button("New", {button_width, button_height}))
		{
			auto editor = m_scene->layers().get<sc::Editor>("Editor");
			if (auto ptr = editor.lock())
			{
				ptr->new_project();
				m_scene->layers().pop();
				m_scene->layers().push("Editor");
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

		ui::imgui_prerender();
		ui::imgui_render();
	}

	const std::string& Menu::get_type() const
	{
		static_assert(true, "Do Not Call.");

		static std::string type = "Menu";
		return type;
	}

	nlohmann::json Menu::serialize()
	{
		return {};
	}

	void Menu::deserialize(const nlohmann::json& json)
	{
	}

	void Menu::load_project()
	{
		auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

		const auto path = fs.open_file_dialog({"*.scproj"}, "../editor_data/projects");
		if (!path.empty())
		{
			auto editor = m_scene->layers().get<sc::Editor>("Editor");
			if (auto ptr = editor.lock())
			{
				ptr->load_project(path);
				m_scene->layers().pop();
				m_scene->layers().push("Editor");
			}
		}
	}
} // namespace sc