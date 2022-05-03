///
/// main.cpp
/// SandboxTests
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/state/SceneManager.hpp>

using namespace galaxy;

class Sandbox : public core::Application
{
public:
	Sandbox()
		: Application {"logs/", "config.json"}
	{
	}

	virtual ~Sandbox()
	{
	}
};

int main(int argsc, char* argsv[])
{
	GALAXY_UNUSED(argsc);
	GALAXY_UNUSED(argsv);

	do
	{
		GALAXY_RESTART  = false;
		GALAXY_START_TP = std::chrono::high_resolution_clock::now();

		try
		{
			Sandbox sandbox;

			{
				auto& sm = core::ServiceLocator<state::SceneManager>::ref();

				auto scene = sm.make("Sandbox");
				sm.set(scene->get_name());

				auto& layerstack = scene->get_stack();

				auto layer = layerstack.make("SandboxLayer");
			}

			sandbox.run();
		}
		catch (const std::exception& e)
		{
			std::cout << "=====================" << std::endl;
			std::cout << "MAIN LOOP EXCEPTION: " << e.what() << std::endl;
			std::cout << "=====================" << std::endl;
		}

	} while (GALAXY_RESTART);

	return EXIT_SUCCESS;
}