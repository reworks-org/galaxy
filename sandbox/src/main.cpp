///
/// main.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <mimalloc-new-delete.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/scene/SceneManager.hpp>

using namespace galaxy;

class Sandbox : public core::Application
{
public:
	Sandbox()
		: Application {"logs/", "config.json"}
	{
	}

	virtual ~Sandbox() = default;
};

int main(int argsc, char* argsv[])
{
	GALAXY_UNUSED(argsc);
	GALAXY_UNUSED(argsv);

	do
	{
		GALAXY_RESTART = false;

		try
		{
			Sandbox app;

			{
				auto& sm = core::ServiceLocator<scene::SceneManager>::ref();
				sm.load_assets();

				auto game    = std::make_shared<scene::Scene>();
				game->m_name = "game";

				sm.add_existing_scene(game->m_name, game);
				sm.set_scene(game->m_name);
			}

			app.run();
		}
		catch (const std::exception& e)
		{
			std::cout << "======================" << std::endl;
			std::cout << " UNHANDLED EXCEPTION: " << e.what() << std::endl;
			std::cout << "======================" << std::endl;
		}
	} while (GALAXY_RESTART);

	return GALAXY_EXIT_SUCCESS;
}