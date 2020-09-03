///
/// FileSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FILESYSTEM_HPP_
#define GALAXY_FILESYSTEM_HPP_

#include <filesystem>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Filesystem paths for various things needing to be loaded.
	///
	class FileSystem final
	{
	public:
		///
		/// Destructor.
		///
		~FileSystem() = default;

		///
		/// Open an open file dialog using pfd.
		///
		/// \param filter See: https://github.com/samhocevar/portable-file-dialogs/blob/master/doc/open_file.md.
		///					Defaults to all files.
		/// \param def_path Default starting path to open dialog at.
		///
		static std::filesystem::path open_file_dialog(const std::string& filter = "*", const std::string& def_path = ".");

		///
		/// Open a save file dialog using pfd.
		///
		/// \param def_path Default starting path to open dialog at.
		///
		static std::filesystem::path save_file_dialog(const std::string& def_path = ".");

		///
		/// Open a folder using a file dialog.
		///
		/// \param def_path Default starting path to open dialog at.
		///
		static std::filesystem::path folder_select_dialog(const std::string& def_path = ".");

		///
		/// Root directory of all files.
		///
		static std::string s_root;

		///
		/// \brief Root directory of textures.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_textures;

		///
		/// \brief Root directory of shaders.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_shaders;

		///
		/// \brief Root directory of scripts.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_scripts;

		///
		/// \brief Root directory of audio.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_audio;

		///
		/// \brief Root directory of json.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_json;

		///
		/// \brief Root directory of fonts.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_fonts;

		///
		/// \brief Root directory of save files.
		///
		/// Should be a subdirectory of s_root.
		///
		static std::string s_saves;

	protected:
		///
		/// Constructor.
		///
		FileSystem() = default;

		///
		/// Copy constructor.
		///
		FileSystem(const FileSystem&) = delete;

		///
		/// Move constructor.
		///
		FileSystem(FileSystem&&) = delete;

		///
		/// Copy assignment operator.
		///
		FileSystem& operator=(const FileSystem&) = delete;

		///
		/// Move assignment operator.
		///
		FileSystem& operator=(FileSystem&&) = delete;
	};
} // namespace galaxy

#endif