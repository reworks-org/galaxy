///
/// main.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <optional>

#include <portable-file-dialogs.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/error/Log.hpp>

#include "layers/Sandbox.hpp"

class SandboxApp : public galaxy::core::Application
{
public:
	SandboxApp(std::string_view asset_dir, std::string_view config_file)
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
			SandboxApp sandbox {"assets/", "assets/config.json"};

			{
				std::shared_ptr<sb::Sandbox> sandbox_instance = std::make_shared<sb::Sandbox>();
				sandbox.set_instance(sandbox_instance);
			}

			restart = sandbox.run();
		}

	} while (restart);

	return 0;
}