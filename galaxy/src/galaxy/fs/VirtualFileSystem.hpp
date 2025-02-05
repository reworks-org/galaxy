///
/// VirtualFileSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_VIRTUALFILESYSTEM_HPP_
#define GALAXY_FS_VIRTUALFILESYSTEM_HPP_

#include <span>
#include <string>
#include <vector>

#include "galaxy/fs/DialogButton.hpp"
#include "galaxy/fs/DialogIcons.hpp"
#include "galaxy/fs/DialogType.hpp"

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
			VirtualFileSystem();

			///
			/// Destructor.
			///
			~VirtualFileSystem();

			///
			/// Read a file.
			///
			/// \param file File to read from.
			///
			/// \return Buffer containing read data.
			///
			[[nodiscard]]
			std::string read(const std::string& file);

			///
			/// Read a binary file.
			///
			/// \param file File to read from.
			///
			/// \return Buffer containing read data.
			///
			[[nodiscard]]
			std::vector<std::uint8_t> read_binary(const std::string& file);

			///
			/// \brief Writes a file to disk.
			///
			/// Checks for existing file to overwrite first.
			///
			/// \param data Data to write to file.
			/// \param file Path to write to, including path from root.
			///
			/// \return True if successful.
			///
			[[nodiscard]]
			bool write(const std::string& data, const std::string& file);

			///
			/// \brief Writes a binary file to disk.
			///
			/// Checks for existing file to overwrite first.
			///
			/// \param data Data to write to file.
			/// \param file Path to write to, including path from root.
			///
			/// \return True if successful.
			///
			[[nodiscard]]
			bool write_binary(std::span<std::uint8_t> data, const std::string& file);

			///
			/// Raw data writing.
			///
			/// \param data Data to write to file.
			/// \param size Size of data to write.
			/// \param file Path to write to, including path from root.
			///
			/// \return True if successful.
			///
			[[nodiscard]]
			bool write_raw(const void* data, const std::size_t size, const std::string& file);

			///
			/// \brief Creates an empty folder in the filesystem relative to the root.
			///
			/// All missing parent directories are also created if they don't exist.
			///
			/// \param dir Directory(s) to create.
			///
			void mkdir(const std::string& dir) noexcept;

			///
			/// \brief Delete a file or folder.
			///
			/// Relative to the root. I.e. "textures/test.png" will become ROOT/textures/test.png.
			///
			/// \param path File or folder to delete.
			///
			void remove(const std::string& path) noexcept;

			///
			/// Does the file exist in the vfs.
			///
			/// \param file File to check.
			///
			/// \return True if file exists.
			///
			[[nodiscard]]
			bool exists(const std::string& file) noexcept;

			///
			/// Checks if a file is a folder.
			///
			/// \param path File/folder to check.
			///
			/// \return True if directory.
			///
			[[nodiscard]]
			bool is_dir(const std::string& path) noexcept;

			///
			/// Get assets in an asset folder.
			///
			/// \param dir Directory to fetch assets from.
			///
			/// \return List of assets in directory.
			///
			[[nodiscard]]
			std::vector<std::string> list(const std::string& dir);

			///
			/// Trigger a standard filesystem audio alert.
			///
			void alert() noexcept;

			///
			/// Trigger a system notification.
			///
			/// \param title Notification title.
			/// \param msg Body text of notification.
			/// \param icon Icon to display alongside text.
			///
			void notification(const std::string& title, const std::string& msg, const DialogIcon icon) noexcept;

			///
			/// Opens a platform specific message box.
			///
			/// \param title Notification title.
			/// \param msg Body text of notification.
			/// \param type Type of message box.
			/// \param icon Icon to display alongside text.
			/// \param btn Button to show on message box.
			///
			/// \return Integer with outcome. 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel.
			///
			[[nodiscard]]
			int message_box(const std::string& title, const std::string& msg, const DialogType type, const DialogIcon icon, const DialogButton btn) noexcept;

			///
			/// Opens a platform specific text input box.
			///
			/// \param title Notification title.
			/// \param msg Body text of notification.
			/// \param default_text Default text displayed in input box.
			/// \param password True to obscure input. Note that enabling this disables default_text.
			///
			/// \return Input as a string.
			///
			[[nodiscard]]
			std::string input_box(const std::string& title, const std::string& msg, const std::string& default_text = "", const bool password = false) noexcept;

			///
			/// Open a save file dialog.
			///
			/// \param default_filename Default name to save file as.
			/// \param filters List of filters for dialog, i.e. "*.png", "*.jpg", etc.
			///
			/// \return String with path, empty if error.
			///
			[[nodiscard]]
			std::string open_save_dialog(const std::string& default_filename, const std::vector<const char*>& filters = {});

			///
			/// Open a file dialog.
			///
			/// \param filters List of filters for dialog, i.e. "*.png", "*.jpg", etc.
			/// \param def_path Default starting path to open dialog at.
			///
			/// \return String with path, empty if error.
			///
			[[nodiscard]]
			std::string open_file_dialog(const std::vector<const char*>& filters = {}, const std::string& def_path = "");

			///
			/// Select a folder using a dialog.
			///
			/// \param def_path Default starting path to open dialog at.
			///
			/// \return String with path, empty if error.
			///
			[[nodiscard]]
			std::string select_folder_dialog(const std::string& def_path = "");

			///
			/// Get pointer to extension for a filename string.
			///
			/// \param file_name File name to parse.
			///
			/// \return File extension, including dot.
			///
			[[nodiscard]]
			std::string get_file_extension(const std::string& file_name) noexcept;

		private:
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
		};
	} // namespace fs
} // namespace galaxy

#endif
