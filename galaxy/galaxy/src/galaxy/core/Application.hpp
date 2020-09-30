///
/// Application.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_APPLICATION_HPP_
#define GALAXY_APPLICATION_HPP_

#include <memory>
#include <span>

#include <frb/Context.hpp>
#include <protostar/async/ThreadPool.hpp>
#include <protostar/state/StateMachine.hpp>
#include <qs/core/Window.hpp>
#include <qs/core/Renderer.hpp>
#include <starlight/Dispatcher.hpp>
#include <sol/forward.hpp>

#include "galaxy/core/World.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/res/FontBook.hpp"
#include "galaxy/res/ShaderBook.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Application superclass. Entry point to run application / game.
	///
	class Application
	{
	public:
		///
		/// Copy constructor.
		///
		Application(const Application&) noexcept = delete;

		///
		/// Move constructor.
		///
		Application(Application&&) noexcept = delete;

		///
		/// Copy assignment operator.
		///
		Application& operator=(const Application&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		Application& operator=(Application&&) noexcept = delete;

		///
		/// \brief Virtual destructor.
		///
		/// Cleans up engine related memory usage.
		///
		virtual ~Application();

		///
		/// Runs the application.
		///
		/// \return Returns true if the program should restart.
		///
		[[maybe_unused]] bool run();

		///
		/// Get delta time.
		///
		/// \return Pointer to protected double.
		///
		[[nodiscard]] pr::ProtectedDouble* get_dt() noexcept;

	protected:
		///
		/// \brief Default constructor.
		///
		/// Sets up the engine. You need to inherit this and call it from a subclass.
		/// Also calls std::srand(std::time(nullptr)) for you.
		///
		/// \param config Configuration file to construct application from.
		///
		explicit Application(std::unique_ptr<galaxy::Config>& config);

		///
		/// Instance of a config reader to parse library config.
		///
		std::unique_ptr<galaxy::Config> m_config;

		///
		/// Threadpool for app.
		///
		std::unique_ptr<pr::ThreadPool<4>> m_threadpool;

		///
		/// Main app window.
		///
		std::unique_ptr<qs::Window> m_window;

		///
		/// Main app renderer.
		///
		std::unique_ptr<qs::Renderer> m_renderer;

		///
		/// Master Lua state for application.
		///
		std::unique_ptr<sol::state> m_lua;

		///
		/// Controls game states.
		///
		std::unique_ptr<pr::StateMachine> m_state;

		///
		/// Process game events.
		///
		std::unique_ptr<sl::Dispatcher> m_dispatcher;

		///
		/// The world class, which manages entities, components, systems, and other important data structures.
		///
		std::unique_ptr<galaxy::World> m_world;

		///
		/// Main serializer.
		///
		//std::unique_ptr<galaxy::Serializer> m_serializer;

		///
		/// Library of all fonts.
		///
		std::unique_ptr<galaxy::FontBook> m_fontbook;

		///
		/// Library of all shaders.
		///
		std::unique_ptr<galaxy::ShaderBook> m_shaderbook;

	private:
		///
		/// Delta Time.
		///
		pr::ProtectedDouble m_delta_time;

		///
		/// OpenAL context.
		///
		frb::Context m_openal;
	};
} // namespace galaxy

#endif