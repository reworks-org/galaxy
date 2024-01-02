///
/// main.cpp
/// dist
///
/// Refer to LICENSE.txt for more details.
///

#include <mimalloc-new-delete.h>
#include <tinyfiledialogs.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/scene/SceneManager.hpp>

using namespace galaxy;

class Dist : public core::Application
{
  public:
	Dist()
		: Application {"logs/", "config.json"}
	{
	}

	virtual ~Dist() = default;
};

int main(int argsc, char* argsv[])
{
	GALAXY_UNUSED(argsc);
	GALAXY_UNUSED(argsv);

	try
	{
		Dist app;

		{
			const auto path = GALAXY_ROOT_DIR / "app.galaxy";

			auto& sm = core::ServiceLocator<scene::SceneManager>::ref();
			sm.load_app(path.string());
		}

		app.run();
	}
	catch (const std::exception& e)
	{
		tinyfd_notifyPopup("Error", e.what(), "error");
	}

	return GALAXY_EXIT_SUCCESS;
}
