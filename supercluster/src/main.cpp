///
/// main.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>

#include "layers/MainMenu.hpp"

class Supercluster : public galaxy::core::Application
{
public:
	Supercluster(std::string_view asset_dir, std::string_view config_file)
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
			Supercluster supercluster {"assets/", "assets/config.json"};

			SL_HANDLE.window()->prevent_native_closing();

			{
				//	std::shared_ptr<sc::Editor> editor      = std::make_shared<sc::Editor>();
				//std::shared_ptr<sc::MainMenu> main_menu = std::make_shared<sc::MainMenu>(&supercluster, editor);

				//supercluster.set_instance(main_menu);
			}

			restart = supercluster.run();
		}

	} while (restart);

	return 0;
}