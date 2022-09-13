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
#include <galaxy/state/Scene.hpp>

#include "Editor.hpp"
#include "../resources/RobotoLight.hpp"

#include "Menu.hpp"

namespace sc
{
	Menu::Menu(std::string_view name, state::Scene* scene) noexcept
		: Layer {name, scene}
	{
		ImGuiIO& io = ImGui::GetIO();

		ImFontConfig font_config         = {};
		font_config.FontDataOwnedByAtlas = false;
		font_config.RasterizerMultiply   = 1.5f;
		font_config.OversampleH          = 4;
		m_bigger_default_font = io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(&ttf::roboto_light), ttf::roboto_light_len, 144.0f, &font_config);
	}

	Menu::~Menu() noexcept
	{
	}

	void Menu::on_push()
	{
	}

	void Menu::on_pop()
	{
	}

	void Menu::events()
	{
	}

	void Menu::update()
	{
	}

	void Menu::render()
	{
		ui::imgui_new_frame();

		static constexpr const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
															   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
		ImGui::Begin("Main Menu", NULL, window_flags);
		ImGui::PopStyleVar(3);

		const auto window_calc             = ImGui::GetWindowWidth() / 2.0f;
		constexpr const auto button_width  = 200.0f;
		constexpr const auto button_height = 40.0f;

		ImGui::PushFont(m_bigger_default_font);
		ImGui::SetCursorPos({window_calc - (ImGui::CalcTextSize("Supercluster Editor").x / 2.0f), 100.0f});
		ImGui::Text("Supercluster Editor");
		ImGui::PopFont();

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
			auto& fs  = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto file = fs.show_open_dialog("*.scproj");
			if (file.has_value())
			{
				auto editor = m_scene->layers().get<sc::Editor>("Editor");
				if (auto ptr = editor.lock())
				{
					ptr->load_project(file.value());
					m_scene->layers().pop();
					m_scene->layers().push("Editor");
				}
			}
		}

		ImGui::Spacing();
		ImGui::SetCursorPosX(window_calc - (button_width / 2.0f));

		if (ImGui::Button("Exit", {button_width, button_height}))
		{
			m_window->close();
		}

		ImGui::End();

		ui::imgui_prerender();
		ui::imgui_render();
	}

	nlohmann::json Menu::serialize()
	{
		return {};
	}

	void Menu::deserialize(const nlohmann::json& json)
	{
	}
} // namespace sc