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
#include "galaxy/core/LayerStack.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/events/dispatcher/Dispatcher.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/graphics/TextureAtlas.hpp"

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
			[[maybe_unused]] const bool run();

		protected:
			///
			/// \brief Default constructor.
			///
			/// Sets up the engine. You need to inherit this and call it from a subclass.
			/// Also calls std::srand(std::time(nullptr)) for you.
			///
			/// \param asset_dir Specify the base directory to mount to the Virtual File System.
			/// \param config_file Name of the config file in the VFS. Can also include a path.
			///
			explicit Application(std::string_view asset_dir, std::string_view config_file);

		protected:
			///
			/// OpenAL context.
			///
			audio::Context m_openal;

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
			std::unique_ptr<Window> m_window;

			///
			/// Main app renderer.
			///
			std::unique_ptr<graphics::Renderer> m_renderer;

			///
			/// Master Lua state for application.
			///
			std::unique_ptr<sol::state> m_lua;

			///
			/// Controls game states (layers).
			///
			std::unique_ptr<LayerStack> m_layerstack;

			///
			/// Process game events.
			///
			std::unique_ptr<events::Dispatcher> m_dispatcher;

			///
			/// Library of all fonts.
			///
			//std::unique_ptr<res::FontBook> m_fontbook;

			///
			/// Library of all shaders.
			///
			//std::unique_ptr<res::ShaderBook> m_shaderbook;

			///
			/// Texture atlas for all textures.
			///
			std::unique_ptr<graphics::TextureAtlas> m_texture_atlas;

			///
			/// Virtual File System.
			///
			std::unique_ptr<fs::Virtual> m_vfs;

		private:
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
		};
	} // namespace core
} // namespace galaxy

#endif