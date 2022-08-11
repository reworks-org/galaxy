///
/// Application.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_APPLICATION_HPP_
#define GALAXY_CORE_APPLICATION_HPP_

#include <string_view>

namespace galaxy
{
	namespace ui
	{
		class RMLFile;
		class RMLSystem;
		class RMLRenderer;
	} // namespace ui

	namespace core
	{
		///
		/// Base level class for any galaxy application.
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
			/// Load scenes into app.
			///
			/// \param json_file Zlib + BAse64 encoded json data file.
			///
			void load(std::string_view json_file);

			///
			/// Runs the application.
			///
			void run();

		protected:
			///
			/// \brief Default constructor.
			///
			/// Sets up the engine. You need to inherit this and call it from a subclass.
			/// Also calls std::srand(std::time(nullptr)) for you.
			///
			/// \param log_dir Specify the base directory to store log files.
			/// \param config_file Filepath of config file.
			///
			explicit Application(std::string_view log_dir, std::string_view config_file);

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

		protected:
			///
			/// RML System interface.
			///
			std::unique_ptr<ui::RMLSystem> m_rml_system_interface;

			///
			/// RML File interface.
			///
			std::unique_ptr<ui::RMLFile> m_rml_file_interface;

			///
			/// RML Rendering interface.
			///
			std::unique_ptr<ui::RMLRenderer> m_rml_rendering_interface;
		};
	} // namespace core
} // namespace galaxy

#endif

/*
		private:
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
	} // namespace core
} // namespace galaxy

#endif
*/