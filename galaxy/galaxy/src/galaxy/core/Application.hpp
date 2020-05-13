///
/// Application.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_APPLICATION_HPP_
#define GALAXY_APPLICATION_HPP_

#include <memory>

#include <sol/forward.hpp>
#include <qs/core/Window.hpp>
#include <qs/renderer/Renderer.hpp>
#include <starlight/Dispatcher.hpp>
#include <protostar/async/ThreadPool.hpp>
#include <protostar/state/StateMachine.hpp>

#include "galaxy/fs/Config.hpp"
#include "galaxy/core/World.hpp"

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
		virtual bool run() final;

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
		std::unique_ptr<protostar::ThreadPool> m_threadPool;

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
		std::unique_ptr<protostar::StateMachine> m_state;

		///
		/// Process game events.
		/// 
		std::unique_ptr<starlight::Dispatcher> m_dispatcher;

		///
		/// The world class, which manages entities, components, systems, and other important data structures.
		///
		std::unique_ptr<galaxy::World> m_world;

		

		///
		/// Main serializer.
		///
		//std::unique_ptr<galaxy::Serializer> m_serializer;

		///
		/// Integrates box2d with the rest of the library.
		///
		//std::unique_ptr<Box2DHelper> m_box2dHelper;

		///
		/// Manages font resources.
		///
		//std::unique_ptr<FontBook> m_fontBook;

		///
		/// Controls and manages music resources.
		///
		//std::unique_ptr<MusicPlayer> m_musicPlayer;
		
		///
		/// Managers shaders and usage of them.
		///
		//std::unique_ptr<ShaderLibrary> m_shaderLibrary;

		///
		/// Controls and manages sound resources.
		///
		//std::unique_ptr<SoundPlayer> m_soundPlayer;

		

		///
		/// Stores and manages all the games graphics. Text, textures, etc.
		///
		//std::unique_ptr<TextureAtlas> m_textureAtlas;
		
		///
		/// Controls the debug UI which allows you to have control over the game from ingame. Console, script editing, etc.
		///
		//std::unique_ptr<DebugInterface> m_debugInterface;

		///
		/// Collision callback functions for Box2D.
		///
		//CollisionContact m_engineCallbacks;

	private:
		///
		/// Delta Time.
		///
		protostar::ProtectedDouble m_timeCorrection;
	};
}

#endif