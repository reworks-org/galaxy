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

#include "galaxy/async/ThreadPool.hpp"
#include "galaxy/audio/Context.hpp"
#include "galaxy/core/Instance.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/Config.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/res/FontBook.hpp"
#include "galaxy/res/Language.hpp"
#include "galaxy/res/MaterialBook.hpp"
#include "galaxy/res/MusicBook.hpp"
#include "galaxy/res/ScriptBook.hpp"
#include "galaxy/res/ShaderBook.hpp"
#include "galaxy/res/SoundBook.hpp"
#include "galaxy/res/TextureAtlas.hpp"

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
			/// Set current instance to process.
			///
			/// \param instance The instance to select.
			///
			void set_instance(std::shared_ptr<Instance> instance);

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
			/// \param file_name The filename that was accessed (not full path).
			/// \param action Action that was performed.
			///
			void reload_assets(FW::WatchID watch_id, const FW::String& dir, const FW::String& file_name, FW::Action action);

		protected:
			///
			/// Application project instance.
			///
			std::shared_ptr<Instance> m_instance;

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
			/// Texture atlas for all textures.
			///
			std::unique_ptr<res::TextureAtlas> m_texture_atlas;

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
			/// Material library.
			///
			std::unique_ptr<res::MaterialBook> m_materialbook;

			///
			/// Language service.
			///
			std::unique_ptr<res::Language> m_language;

			///
			/// Threadpool.
			///
			std::unique_ptr<async::ThreadPool> m_pool;

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