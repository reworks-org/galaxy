///
/// main.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <portable-file-dialogs.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/error/Log.hpp>

#include "layers/EditorLayer.hpp"

class Editor : public galaxy::core::Application
{
public:
	Editor(std::string_view asset_dir, std::string_view config_file)
	    : Application {asset_dir, config_file}
	{
	}
};

int main(int argsc, char* argsv[])
{
	bool restart = false;

	do
	{
		restart             = false;
		SL_HANDLE.m_restart = false;

		try
		{
			Editor editor {"assets/", "assets/config.json"};

			auto* stack = SL_HANDLE.layerstack();
			stack->create<sc::EditorLayer>("Editor");
			stack->push("Editor");

			restart = editor.run();
		}
		catch (std::exception& e)
		{
			pfd::message message("Exception Caught", e.what(), pfd::choice::ok, pfd::icon::error);
		}
		catch (std::exception* e)
		{
			pfd::message message("Exception Caught", e->what(), pfd::choice::ok, pfd::icon::error);
		}

	} while (restart);

	return 0;
}