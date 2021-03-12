///
/// main.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <optional>

#include <portable-file-dialogs.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/error/Log.hpp>

#include "instances/Editor.hpp"

class EditorApp : public galaxy::core::Application
{
public:
	EditorApp(std::string_view asset_dir, std::string_view config_file)
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

		{
			EditorApp editor {"assets/", "assets/config.json"};
			SL_HANDLE.window()->prevent_native_closing();

			{
				std::shared_ptr<sc::Editor> instance = std::make_shared<sc::Editor>();
				editor.set_instance(instance);
			}

			restart = editor.run();
		}

	} while (restart);

	return 0;
}