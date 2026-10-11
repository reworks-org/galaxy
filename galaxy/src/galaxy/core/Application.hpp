///
/// Application.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_APPLICATION_HPP_
#define GALAXY_CORE_APPLICATION_HPP_

#include <string>
#include <functional>

#include <SDL3/SDL_events.h>

namespace galaxy
{
	///
	/// Base level class for any galaxy app.
	///
	class App final
	{
		///
		/// Defines a callback for update() or render() loops in app.run().
		///
		using LoopFunc = std::move_only_function<void(App* app)>;

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

		///
		/// Use a custom update step in game loop.
		///
		/// \param update Callback.
		///
		void set_update_func(LoopFunc&& update);

		///
		/// Use a custom rendering step in game loop.
		///
		/// \param render Callback.
		///
		void set_render_func(LoopFunc&& render);

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
		void setup_logging();
		void setup_config(std::string_view config_file);
		void setup_platform();
		void setup_fs();
		void setup_rendering();
		void setup_events();
		void setup_input();
		// void setup_nuklear();
		// void setup_loader();
		void setup_meta();
		void setup_services();
		void setup_scripting();

	private:
		///
		/// Core event data.
		///
		SDL_Event m_events;

		///
		/// Update step in gameloop.
		///
		LoopFunc m_update;

		///
		/// Render step in gameloop.
		///
		LoopFunc m_render;
	};
} // namespace galaxy

#endif
