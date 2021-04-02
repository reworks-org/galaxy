///
/// main.cpp
/// sandbox_3d_tests
///
/// Refer to LICENSE.txt for more details.
///

#include <optional>

#include <portable-file-dialogs.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/error/Log.hpp>

#include "Sandbox3D.hpp"

using namespace galaxy;

class Sandbox3DApp : public core::Application
{
public:
	Sandbox3DApp(std::string_view asset_dir, std::string_view config_file)
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
			Sandbox3DApp sandbox_3d {"assets/", "assets/config.json"};

			{
				auto sandbox_3d_instance = std::make_shared<s3d::Sandbox3D>();
				sandbox_3d.set_instance(sandbox_3d_instance);
			}

			restart = sandbox_3d.run();
		}

	} while (restart);

	return 0;
}