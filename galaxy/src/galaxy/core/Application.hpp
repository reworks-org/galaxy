///
/// Application.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_APPLICATION_HPP_
#define GALAXY_CORE_APPLICATION_HPP_

#include <string>

#include <SDL3/SDL_events.h>

namespace galaxy
{
	///
	/// Base level class for any galaxy app.
	///
	class App final
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Constructs the engine, services, and loads resources.
		///
		/// \param config_file Filepath of config file.
		///
		explicit App(const std::string& config_file = "config.json");

		///
		/// Destructor.
		///
		~App();

		///
		/// \brief Load app data file into scene manager.
		///
		/// Will remove any other loaded scenes and adjust the currently loaded scene.
		///
		/// \param app_data Zlib + Base64 encoded json data file. Should have .galaxy extension.
		///
		void load(const std::string& app_data = "base.galaxy");

		///
		/// Save all active scenes and sub data within those scenes.
		///
		/// \param app_data File to save data to.
		///
		void save(const std::string& app_data);

		///
		/// Main game loop.
		///
		void run();

		///
		/// Get event data.
		///
		/// \return SDL_Event data.
		///
		SDL_Event& events() noexcept;

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

		void setup_async();
		void setup_config(std::string_view config_file);
		void setup_logging();
		void setup_platform();
		void setup_fs();
		void setup_rendering();
		void setup_input();
		void setup_factory();
		// void setup_loader();
		// void setup_meta();
		void setup_services();
		void setup_ui();
		void setup_scripting();

	private:
		///
		/// Core event data.
		///
		SDL_Event m_events;
	};
} // namespace galaxy

#endif
