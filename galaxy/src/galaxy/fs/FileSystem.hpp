///
/// FileSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_FILESYSTEM_HPP_
#define GALAXY_FS_FILESYSTEM_HPP_

#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string_view>

#include <FileWatcher/FileWatcher.h>

#define CUR_DIR std::filesystem::current_path().string()

namespace galaxy
{
	namespace core
	{
		class Application;
	} // namespace core

	namespace fs
	{
		///
		/// Virtual File System to make managing files easier.
		///
		class Virtual final
		{
			friend class core::Application;

		public:
			///
			/// Constructor.
			///
			Virtual() noexcept = default;

			///
			/// Destructor.
			///
			~Virtual() noexcept;

			///
			/// \brief Creates a blank file in the filesystem.
			///
			/// WARNING: THIS WILL OVERWRITE AN EXISTING FILE.
			///
			/// \param filepath Path and filename to create file. I.e. "assets/test.txt".
			///
			void create_file(std::string_view filepath);

			///
			/// Open a file and store contents in std::string.
			///
			/// \param file File to open.
			///
			/// \return Buffer in a std::string format holding read info.
			///
			[[nodiscard]] std::optional<std::string> open(std::string_view file);

			///
			/// Open a binary file and store in std::span.
			///
			/// \param file File to open.
			///
			/// \return Span buffer.
			///
			[[nodiscard]] std::optional<std::vector<char>> open_binary(std::string_view file);

			///
			/// \brief Saves a string to a file.
			///
			/// Checks for existing file to overwrite first.
			///
			/// \param data Data to write to file.
			/// \param file File to save to.
			///
			/// \return Const bool. True if successful.
			///
			[[nodiscard]] const bool save(const std::string& data, std::string_view file);

			///
			/// \brief Saves binary data to a file.
			///
			/// Checks for existing file to overwrite first.
			///
			/// \param data Data to write to file.
			/// \param file File to overwrite.
			///
			/// \return Const bool. True if successful.
			///
			[[nodiscard]] const bool save_binary(std::span<char> data, std::string_view file);

			///
			/// \brief Retrieve the absolute position of a file to load.
			///
			/// Searches recursively over each mount point.
			///
			/// \param file Name of file to get path for.
			///
			[[nodiscard]] std::optional<std::string> absolute(std::string_view file);

			///
			/// Open an open file dialog using pfd.
			///
			/// \param filter See: https://github.com/samhocevar/portable-file-dialogs/blob/master/doc/open_file.md.
			///					Defaults to all files.
			/// \param def_path Default starting path to open dialog at.
			///
			[[nodiscard]] std::optional<std::string> show_open_dialog(const std::string& filter = "*", const std::string& def_path = CUR_DIR);

			///
			/// Open a save file dialog using pfd.
			///
			/// \param filter See: https://github.com/samhocevar/portable-file-dialogs/blob/master/doc/open_file.md.
			///					Defaults to all files.
			/// \param def_path Default starting path to open dialog at.
			///
			[[nodiscard]] std::optional<std::string> show_save_dialog(const std::string& filter = "*", const std::string& def_path = CUR_DIR);

			///
			/// Open a folder using a file dialog.
			///
			/// \param def_path Default starting path to open dialog at.
			///
			[[nodiscard]] std::optional<std::string> show_folder_dialog(const std::string& def_path = CUR_DIR);

			///
			/// Open a file using a dialog.
			///
			/// \param filter See: https://github.com/samhocevar/portable-file-dialogs/blob/master/doc/open_file.md.
			///					Defaults to all files.
			/// \param def_path Default starting path to open dialog at.
			///
			[[nodiscard]] std::optional<std::string> open_with_dialog(const std::string& filter = "*", const std::string& def_path = CUR_DIR);

			///
			/// Save a file using a dialog.
			///
			/// \param data Data to write to file.
			/// \param def_path Default starting path to open dialog at.
			///
			/// \return Const bool. True if successful.
			///
			[[nodiscard]] const bool save_with_dialog(const std::string& data, const std::string& def_path = CUR_DIR);

		private:
			///
			/// Update listener for filesystem changes.
			///
			class UpdateListener final : public FW::FileWatchListener
			{
			public:
				///
				/// Constructor.
				///
				UpdateListener() noexcept = default;

				///
				/// Virtual destructor.
				///
				virtual ~UpdateListener() noexcept = default;

				///
				/// Handles the action file action.
				///
				/// \param watch_id The watch id for the directory.
				/// \param dir The directory.
				/// \param file_name The filename that was accessed (not full path).
				/// \param action Action that was performed.
				///
				void handleFileAction(FW::WatchID watch_id, const FW::String& dir, const FW::String& file_name, FW::Action action) override;

			public:
				///
				/// Callback set by application.
				///
				std::function<void(FW::WatchID, const FW::String&, const FW::String&, FW::Action)> m_on_file_change;
			};

		private:
			///
			/// Mounts a directory to the VFS.
			///
			/// \param dir Directory to add.
			///
			/// \return Returns false if not a directory, and does not mount if so.
			///
			[[maybe_unused]] const bool mount(std::string_view dir);

		private:
			///
			/// Stores mounted directories.
			///
			std::vector<std::filesystem::path> m_dirs;

			///
			/// The listener implementation.
			///
			UpdateListener m_listener;

			///
			/// The filewatcher that listens for changes.
			///
			FW::FileWatcher m_watcher;
		};
	} // namespace fs
} // namespace galaxy

#endif