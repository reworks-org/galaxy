///
/// main.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <mimalloc.h>
#include <mimalloc-new-delete.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// #include <galaxy/core/Application.hpp>
// #include <galaxy/core/Config.hpp>
// #include <galaxy/core/ServiceLocator.hpp>
// #include <galaxy/core/Window.hpp>
// #include <galaxy/fs/VirtualFileSystem.hpp>
// #include <galaxy/ui/ImGuiHelpers.hpp>

// #include "Editor.hpp"

// using namespace galaxy;

int main(int argc, char* argv[])
{
	mi_version();
	// 	GALAXY_UNUSED(argsc);
	// 	GALAXY_UNUSED(argsv);

	// 	try
	// 	{
	// 		core::App supercluster("logs/", "config.json");

	// 		{
	// 			auto data = core::ServiceLocator<fs::VirtualFileSystem>::ref().read_binary("sc.png");

	// 			auto& window = core::ServiceLocator<core::Window>::ref();
	// 			window.set_icon(data);

	// 			if (!std::filesystem::exists(GALAXY_ROOT_DIR / GALAXY_EDITOR_DATA_DIR / "projects"))
	// 			{
	// 				std::filesystem::create_directory(GALAXY_ROOT_DIR / GALAXY_EDITOR_DATA_DIR / "projects");
	// 			}

	// 			ui::imgui_init_context("layout.ini");

	// 			auto& sm = core::ServiceLocator<scene::SceneManager>::ref();
	// 			sm.add_custom<sc::Editor>("sc_editor");
	// 			sm.set_scene("sc_editor");
	// 		}

	// 		supercluster.run();
	// 		ui::imgui_destroy_context();
	// 	}
	// 	catch (const std::exception& e)
	// 	{
	// 		std::cout << "======================" << std::endl;
	// 		std::cout << " UNHANDLED EXCEPTION: " << e.what() << std::endl;
	// 		std::cout << "======================" << std::endl;
	// 		std::cin.get();
	// 	}

	// 	return GALAXY_EXIT_SUCCESS;
	return 0;
}

/*
				draw_component<components::GUI>(selected, "GUI", [&](components::GUI* gui) {
					if (ImGui::Button("Load"))
					{
						const auto path = core::ServiceLocator<fs::VirtualFileSystem>::ref().open_file_dialog({"*.lua"});
						if (!path.empty())
						{
							auto str = "{\"file\":\"" + path + "\"}";
							strutils::replace_all(str, "\\", "/");
							selected.scene->m_registry.m_entt.emplace_or_replace<components::GUI>(selected.entity, nlohmann::json::parse(str));
						}
					}
					ImGui::SameLine();
					if (ImGui::Button("Reload"))
					{
						if (!gui->file().empty())
						{
							auto str = "{\"file\":\"" + gui->file() + "\"}";
							strutils::replace_all(str, "\\", "/");
							selected.scene->m_registry.m_entt.emplace_or_replace<components::GUI>(selected.entity, nlohmann::json::parse(str));
						}
					}
					if (ImGui::CollapsingHeader("Data", ImGuiTreeNodeFlags_SpanAvailWidth))
					{
						if (gui->m_self.valid())
						{
							for (auto& [id, value] : gui->m_self)
							{
								const auto name = id.as<std::string>();
								switch (value.get_type())
								{
									case sol::type::number:
										{
											auto num = value.as<double>();
											if (ImGui::InputDouble(name.c_str(),
													&num,
													0.1,
													1.0,
													"%.1f",
													ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank))
											{
												gui->m_self[name] = num;
											}
										}
										break;
									case sol::type::boolean:
										{
											auto var = value.as<bool>();
											if (ImGui::Checkbox(name.c_str(), &var))
											{
												gui->m_self[name] = var;
											}
										}
										break;
									case sol::type::string:
										{
											auto str = value.as<std::string>();
											if (ImGui::InputText(name.c_str(), &str, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
											{
												gui->m_self[name] = str;
											}
										}
										break;
									case sol::type::function:
										ImGui::Text("Function: %s", name.c_str());
										break;
									case sol::type::userdata:
									case sol::type::lightuserdata:
										ImGui::Text("Userdata: %s", name.c_str());
										break;
									default:
										{
											const auto type = std::string {magic_enum::enum_name(value.get_type())};
											ImGui::Text("%s: %s", type.c_str(), name.c_str());
										}
										break;
								}
							}
						}
					}
				});

*/
