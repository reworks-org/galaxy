///
/// main.cpp
/// sandox
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <galaxy/core/Application.hpp>
#include <galaxy/utils/Globals.hpp>

using namespace galaxy;

class Sandbox : public core::Application
{
public:
	Sandbox()
		: Application {"logs/", "config.json"}
	{
	}

	virtual ~Sandbox() noexcept = default;
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