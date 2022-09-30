///
/// VirtualFileSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_VIRTUALFILESYSTEM_HPP_
#define GALAXY_FS_VIRTUALFILESYSTEM_HPP_

#include <span>

#include "galaxy/fs/FileInfo.hpp"
#include "galaxy/utils/Globals.hpp"

namespace galaxy
{
	namespace fs
	{
		///
		/// File System to make managing files easier.
		///
		class VirtualFileSystem final
		{
		public:
			///
			/// Constructor.
			///
			/// \param root Root folder to hold vfs in.
			///
			VirtualFileSystem(std::string_view root);

			///
			/// Destructor.
			///
			~VirtualFileSystem() noexcept = default;

			///
			/// Retrieve the absolute position of a file to load.
			///
			/// \param file File to get the absolute path of within the VFS. If contains folder, will search for that folder, then look in that folder for file.
			/// I.e. audio/test.ogg as the filename, will look for files in audio/ and audio/music, audio/sfx, etc, returning first match, but
			/// audio/music/test.ogg would only look in audio/music/.
			/// Note that audio/ must be top level, i.e. right under root, otherwise directory wont be found.
			/// So in FS it would look like: root/audio/music/test.ogg.
			/// If you use audio/test.ogg and the physical path is root/woops/audio/music/test.ogg, file wont be found because audio/ is not under root/.
			///
			/// \return Path as a filesystem, string and a code to process what happened.
			///
			[[nodiscard]] FileInfo find(std::string_view file);

			///
			/// \brief Creates an empty file in the filesystem relative to the root.
			///
			/// \param path Name of file to create. Can contain a path. I.e. "assets/test.txt".
			/// \param binary Create the file as a binary file.
			///
			/// \return True if creation successful, false if failed or file already exists.
			///
			[[nodiscard]] bool create_file(std::string_view path, const bool binary);

			///
			/// \brief Creates an empty folder in the filesystem relative to the root.
			///
			/// \param path Name of folder to create. Can contain a path. I.e. "assets/dir".
			///
			/// \return True if creation successful, false if failed or folder already exists.
			///
			[[nodiscard]] bool create_folder(std::string_view path);

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
			[[nodiscard]] bool save(const std::string& data, std::string_view file);

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
			[[nodiscard]] bool save_binary(std::span<char> data, std::string_view file);

			///
			/// \brief Delete a file or folder.
			///
			/// Does not delete directories in path.
			///
			/// \param path File or Folder to delete.
			///
			void remove(std::string_view path);

			///
			/// Open an open file dialog using pfd.
			///
			/// \param filter See: https://github.com/samhocevar/portable-file-dialogs/blob/master/doc/open_file.md.
			///					Defaults to all files.
			/// \param def_path Default starting path to open dialog at.
			///
			/// \return String with path, nullopt if dialog closed or nothing selected.
			///
			[[nodiscard]] std::optional<std::string> show_open_dialog(const std::string& filter = "*", const std::string& def_path = "");

			///
			/// Opens a file dialog to select a list of files to open.
			///
			/// \param filter See: https://github.com/samhocevar/portable-file-dialogs/blob/master/doc/open_file.md.
			///					Defaults to all files.
			/// \param def_path Default starting path to open dialog at.
			///
			/// \return List of paths, nullopt if dialog closed or nothing selected.
			///
			[[nodiscard]] std::optional<std::vector<std::string>> show_open_dialog_list(const std::string& filter = "*", const std::string& def_path = "");

			///
			/// Open a save file dialog using pfd.
			///
			/// \param filter See: https://github.com/samhocevar/portable-file-dialogs/blob/master/doc/open_file.md.
			///					Defaults to all files.
			/// \param def_path Default starting path to open dialog at.
			///
			/// \return String with path, nullopt if dialog closed or nothing selected.
			///
			[[nodiscard]] std::optional<std::string> show_save_dialog(const std::string& filter = "*", const std::string& def_path = "");

			///
			/// Open a folder using a file dialog.
			///
			/// \param def_path Default starting path to open dialog at.
			///
			/// \return String with path, nullopt if dialog closed or nothing selected.
			///
			[[nodiscard]] std::optional<std::string> show_folder_dialog(const std::string& def_path = "");

			///
			/// Open a file using a dialog.
			///
			/// \param filter See: https://github.com/samhocevar/portable-file-dialogs/blob/master/doc/open_file.md.
			///					Defaults to all files.
			/// \param def_path Default starting path to open dialog at.
			///
			/// \return String with path, nullopt if dialog closed or nothing selected.
			///
			[[nodiscard]] std::optional<std::string> open_with_dialog(const std::string& filter = "*", const std::string& def_path = "");

			///
			/// Save a file using a dialog.
			///
			/// \param data Data to write to file.
			/// \param def_path Default starting path to open dialog at.
			///
			/// \return Const bool. True if successful.
			///
			[[nodiscard]] bool save_with_dialog(const std::string& data, const std::string& def_path = "");

			///
			/// List contents of a directory in the VFS recursively.
			///
			/// \param path Path, including directory, in the VFS.
			///
			/// \return List of files / folders in directory. Empty if there is an error / nothing found.
			///
			[[nodiscard]] std::vector<std::string> list_directory(std::string_view path);

			///
			/// Get root path.
			///
			/// \return Absolute root path as a string.
			///
			[[nodiscard]] std::string root() noexcept;

			///
			/// Get root path.
			///
			/// \return Absolute root path as a filesystem path.
			///
			[[nodiscard]] const std::filesystem::path& root_path() const noexcept;

		private:
			///
			/// Constructor.
			///
			VirtualFileSystem() = delete;

			///
			/// Copy constructor.
			///
			VirtualFileSystem(const VirtualFileSystem&) = delete;

			///
			/// Move constructor.
			///
			VirtualFileSystem(VirtualFileSystem&&) = delete;

			///
			/// Copy assignment operator.
			///
			VirtualFileSystem& operator=(const VirtualFileSystem&) = delete;

			///
			/// Move assignment operator.
			///
			VirtualFileSystem& operator=(VirtualFileSystem&&) = delete;

		private:
			///
			/// Root asset path location.
			///
			std::filesystem::path m_root;
		};
	} // namespace fs
} // namespace galaxy

#endif