///
/// main.cpp
/// sandbox_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>

#include "Sandbox.hpp"

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
	GALAXY_UNUSED(argsc);
	GALAXY_UNUSED(argsv);

	bool restart = false;

	do
	{
		restart                          = false;
		SL_HANDLE.m_restart              = false;
		SL_HANDLE.m_app_start_time_point = std::chrono::high_resolution_clock::now();

		{
			SandboxApp app {"assets/", "assets/config.json"};

			auto sandbox_layer = app.create_layer<sb::Sandbox>();
			app.push_layer(sandbox_layer);

			restart = app.run();
		}

	} while (restart);

	return EXIT_SUCCESS;
}