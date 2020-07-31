///
/// Application.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_APPLICATION_HPP_
#define GALAXY_APPLICATION_HPP_

#include <memory>

#include <frb/Context.hpp>
#include <sol/forward.hpp>
#include <qs/core/Window.hpp>
#include <qs/core/Renderer.hpp>
#include <starlight/Dispatcher.hpp>
#include <protostar/async/ThreadPool.hpp>
#include <protostar/state/StateMachine.hpp>

#include "galaxy/core/World.hpp"

#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/Serializer.hpp"

#include "galaxy/res/FontBook.hpp"
#include "galaxy/res/AudioBook.hpp"
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
		virtual bool run() noexcept final;

		///
		/// Get delta time.
		///
		/// \return Pointer to protected double.
		///
		virtual pr::ProtectedDouble* getDT() noexcept final;

	protected:
		///
		/// \brief Default constructor.
		///
		/// Sets up the engine. You need to inherit this and call it from a subclass.
		/// Also calls std::srand(std::time(nullptr)) for you.
		/// 
		/// \param config You need to construct, initialize and open a config file.
		///               std::move() is called for you.
		///
		Application(std::unique_ptr<galaxy::Config>& config);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Application() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		Application(const Application&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		Application(Application&&) = delete;

	protected:
		///
		/// Instance of a config reader to parse library config.
		///
		std::unique_ptr<galaxy::Config> m_config;
		
		///
		/// Threadpool for app.
		///
		std::unique_ptr<pr::ThreadPool> m_threadPool;

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
		std::unique_ptr<galaxy::Serializer> m_serializer;

		///
		/// Library of all fonts.
		///
		std::unique_ptr<galaxy::FontBook> m_fontbook;

		///
		/// Library of all shaders.
		///
		std::unique_ptr<galaxy::ShaderBook> m_shaderbook;

		///
		/// Library of all audio.
		///
		std::unique_ptr<galaxy::AudioBook> m_audiobook;
		
	private:
		///
		/// Delta Time.
		///
		pr::ProtectedDouble m_timeCorrection;

		///
		/// OpenAL context.
		///
		frb::Context m_alContext;
	};
}

#endif