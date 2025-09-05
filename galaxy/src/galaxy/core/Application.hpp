///
/// Application.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_APPLICATION_HPP_
#define GALAXY_CORE_APPLICATION_HPP_

#include <string>

#include <entt/signal/fwd.hpp>

namespace galaxy
{
	class Window;

	///
	/// Base level class for any galaxy app.
	///
	class App final
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Sets up the engine. You need to inherit this and call it from a subclass.
		/// Also calls std::srand(std::time(nullptr)) for you.
		///
		/// \param config_file Filepath of config file.
		///
		explicit App(const std::string& config_file = "config.json");

		///
		/// Destructor.
		///
		~App();

		///
		/// Loads the default appdata file.
		///
		// void load();

		///
		/// Main game loop.
		///
		void run();

	private:
		///
		/// Copy constructor.
		///
		App(const App&) = delete;

		///
		/// Move constructor.
		///
		App(App&&) = delete;

		///
		/// Copy assignment operator.
		///
		App& operator=(const App&) = delete;

		///
		/// Move assignment operator.
		///
		App& operator=(App&&) = delete;

		void handle_events(Window& window, entt::dispatcher& dispatcher);

		void setup_logging();
		void setup_async();
		void setup_config(std::string_view config_file);
		void setup_platform();
		void setup_fs();
		void setup_window();
		void setup_events();
		// void setup_nuklear();
		// void setup_loader();
		// void setup_meta();
		// void setup_scripting();
		// void setup_services();
	};
} // namespace galaxy

#endif
