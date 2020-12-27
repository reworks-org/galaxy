///
/// Application.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_APPLICATION_HPP_
#define GALAXY_CORE_APPLICATION_HPP_

#include <memory>
#include <span>

#include <sol/forward.hpp>

#include "galaxy/audio/Context.hpp"
#include "galaxy/async/ThreadPool.hpp"
#include "galaxy/core/World.hpp"
#include "galaxy/core/StateMachine.hpp"
#include "galaxy/events/dispatcher/Dispatcher.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/Serializer.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/graphics/TextureAtlas.hpp"
#include "galaxy/graphics/Window.hpp"
#include "galaxy/res/FontBook.hpp"
#include "galaxy/res/ShaderBook.hpp"
#include "galaxy/tools/DevTools.hpp"

namespace galaxy
{
	namespace core
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
			Application(const Application&) = delete;

			///
			/// Move constructor.
			///
			Application(Application&&) = delete;

			///
			/// Copy assignment operator.
			///
			Application& operator=(const Application&) = delete;

			///
			/// Move assignment operator.
			///
			Application& operator=(Application&&) = delete;

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

		protected:
			///
			/// \brief Default constructor.
			///
			/// Sets up the engine. You need to inherit this and call it from a subclass.
			/// Also calls std::srand(std::time(nullptr)) for you.
			///
			/// \param config Configuration file to construct application from.
			///
			explicit Application(std::unique_ptr<fs::Config>& config);

			///
			/// Instance of a config reader to parse library config.
			///
			std::unique_ptr<fs::Config> m_config;

			///
			/// Threadpool for app.
			///
			std::unique_ptr<async::ThreadPool<4>> m_threadpool;

			///
			/// Main app window.
			///
			std::unique_ptr<graphics::Window> m_window;

			///
			/// Main app renderer.
			///
			std::unique_ptr<graphics::Renderer> m_renderer;

			///
			/// Master Lua state for application.
			///
			std::unique_ptr<sol::state> m_lua;

			///
			/// Controls game states.
			///
			std::unique_ptr<StateMachine> m_state;

			///
			/// Process game events.
			///
			std::unique_ptr<events::Dispatcher> m_dispatcher;

			///
			/// The world class, which manages entities, components, systems, and other important data structures.
			///
			std::unique_ptr<World> m_world;

			///
			/// Main serializer.
			///
			std::unique_ptr<fs::Serializer> m_serializer;

			///
			/// Library of all fonts.
			///
			std::unique_ptr<res::FontBook> m_fontbook;

			///
			/// Library of all shaders.
			///
			std::unique_ptr<res::ShaderBook> m_shaderbook;

			///
			/// Texture atlas for all textures.
			///
			std::unique_ptr<graphics::TextureAtlas> m_texture_atlas;

		private:
			///
			/// Allow for devtools visibility.
			///
			bool m_visible_tools;

			///
			/// OpenAL context.
			///
			audio::Context m_openal;

			///
			/// Dev Tools.
			///
			tools::DevTools m_tools;
		};
	} // namespace core
} // namespace galaxy

#endif