///
/// Application.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_APPLICATION_HPP_
#define GALAXY_CORE_APPLICATION_HPP_

#include <stack>

#include <sol/forward.hpp>

#include "galaxy/async/ThreadPool.hpp"
#include "galaxy/audio/Context.hpp"
#include "galaxy/core/Layer.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/FileListener.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/resource/FontBook.hpp"
#include "galaxy/resource/Language.hpp"
#include "galaxy/resource/MusicBook.hpp"
#include "galaxy/resource/ScriptBook.hpp"
#include "galaxy/resource/ShaderBook.hpp"
#include "galaxy/resource/SoundBook.hpp"
#include "galaxy/resource/TextureBook.hpp"

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
			/// Create an application layer.
			///
			/// \return Shared pointer to layer. Stored internally to preserve lifetime.
			///
			template<std::derived_from<Layer> DerivedLayer>
			[[nodiscard]] std::shared_ptr<DerivedLayer> create_layer();

			///
			/// Push a layer to the top of the stack.
			///
			/// \param layer Pointer to layer.
			///
			void push_layer(std::shared_ptr<Layer> layer);

			///
			/// Pop topmost layer.
			///
			void pop_layer();

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

			///
			/// \brief Create default asset layout.
			///
			/// This uses a const string reference over string_view since
			/// we need to combine the strings into a new string.
			///
			/// \param root Root asset folder.
			/// \param asset_folder Path to the asset folder to create.
			///
			void create_asset_layout(const std::string& root, const std::string& asset_folder);

			///
			/// Generate default required files in asset folders.
			///
			/// \param root Root directory. Is not a string_view because we need to merge it.
			///
			void generate_default_assets(const std::string& root);

			///
			/// Reload assets from disk.
			///
			/// \param watch_id The watch id for the directory.
			/// \param dir The directory.
			/// \param filename The filename that was accessed (not full path).
			/// \param action Action that was performed.
			/// \param old_filename The name of the file or directory moved.
			///
			void reload_assets(efsw::WatchID watch_id, const std::string& dir, const std::string& filename, efsw::Action action, std::string old_filename);

		protected:
			///
			/// Layer stack.
			///
			std::stack<std::shared_ptr<Layer>> m_layer_stack;

			///
			/// Layer storage.
			///
			std::vector<std::shared_ptr<Layer>> m_layers;

			///
			/// OpenAL context.
			///
			audio::Context m_openal;

			///
			/// Instance of a config reader to parse library config.
			///
			std::unique_ptr<fs::Config> m_config;

			///
			/// Main app window.
			///
			std::unique_ptr<Window> m_window;

			///
			/// Master Lua state for application.
			///
			std::unique_ptr<sol::state> m_lua;

			///
			/// Library of all fonts.
			///
			std::unique_ptr<res::FontBook> m_fontbook;

			///
			/// Library of all shaders.
			///
			std::unique_ptr<res::ShaderBook> m_shaderbook;

			///
			/// Texture manager for all textures.
			///
			std::unique_ptr<res::TextureBook> m_texturebook;

			///
			/// Library of all sounds.
			///
			std::unique_ptr<res::SoundBook> m_soundbook;

			///
			/// Library of all music.
			///
			std::unique_ptr<res::MusicBook> m_musicbook;

			///
			/// Virtual File System.
			///
			std::unique_ptr<fs::Virtual> m_vfs;

			///
			/// Script library.
			///
			std::unique_ptr<res::ScriptBook> m_scriptbook;

			///
			/// Language service.
			///
			std::unique_ptr<res::Language> m_langs;

			///
			/// Threadpool.
			///
			std::unique_ptr<async::ThreadPool> m_pool;

		private:
			///
			/// Filesystem watcher.
			///
			efsw::FileWatcher m_filewatcher;

			///
			/// Filesystem listener.
			///
			std::unique_ptr<fs::FileListener> m_filelistener;
		};

		template<std::derived_from<Layer> DerivedLayer>
		inline std::shared_ptr<DerivedLayer> Application::create_layer()
		{
			std::shared_ptr<DerivedLayer> layer = std::make_shared<DerivedLayer>();
			m_layers.emplace_back(std::static_pointer_cast<Layer>(layer));

			return layer;
		}
	} // namespace core
} // namespace galaxy

#endif