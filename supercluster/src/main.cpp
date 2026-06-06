///
/// main.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

// #include <filesystem>

// #include <entt/locator/locator.hpp>
#include <mimalloc.h>
#include <mimalloc-new-delete.h>

// #include <physfs.h>
// #include <tinyfiledialogs.h>

// #include <galaxy/core/Application.hpp>
// #include <galaxy/core/Window.hpp>
// #include <galaxy/core/Settings.hpp>
// #include <galaxy/logging/PhysFSError.hpp>
// #include <galaxy/platform/Pragma.hpp>
// #include <galaxy/scene/SceneManager.hpp>
// #include <galaxy/ui/ImGuiHelpers.hpp>
//
// #include "Editor.hpp"
//
// using namespace galaxy;
//
// void create_projects()
//{
//	const auto merged = Settings::root_dir() / Settings::editor_dir();
//	if (!std::filesystem::exists(merged))
//	{
//		std::filesystem::create_directories(merged);
//	}
//
//	const auto projects = merged / "projects";
//	if (!std::filesystem::exists(projects))
//	{
//		std::filesystem::create_directories(projects);
//	}
//
//	log::physfs_check(PHYSFS_mount(merged.string().c_str(), "editor", true));
// }
//
// void set_icon()
//{
//	auto& window = entt::locator<Window>::value();
//	window.set_icon("editor/sc.png");
// }

int main(int argc, char* argv[])
{
	mi_version();

	// GALAXY_UNUSED(argc);
	// GALAXY_UNUSED(argv);

	// try
	//{
	//	auto app    = galaxy::App();
	//	auto editor = sc::Editor();

	//	create_projects();
	//	set_icon();

	//	app.set_update_func([&](App* app) {
	//		editor.update(app->events());
	//	});

	//	app.set_render_func([&](App* app) {
	//		editor.render();
	//	});

	//	app.run();
	//}
	// catch (const std::exception& e)
	//{
	//	tinyfd_notifyPopup("Error", e.what(), "error");
	//}

	return 0;
}
