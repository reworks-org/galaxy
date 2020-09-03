///
/// FileSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <portable-file-dialogs.h>

#include "FileSystem.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	std::string FileSystem::s_root;
	std::string FileSystem::s_textures;
	std::string FileSystem::s_shaders;
	std::string FileSystem::s_scripts;
	std::string FileSystem::s_audio;
	std::string FileSystem::s_json;
	std::string FileSystem::s_fonts;
	std::string FileSystem::s_saves;

	std::filesystem::path FileSystem::open_file_dialog(const std::string& filter, const std::string& def_path)
	{
		std::filesystem::path path {""};
		auto selection = pfd::open_file("Open a file.", def_path, {"Files", filter}).result();

		if (!selection.empty())
		{
			path = selection[0];
		}

		return path;
	}

	std::filesystem::path FileSystem::save_file_dialog(const std::string& def_path)
	{
		std::filesystem::path path {""};
		auto destination = pfd::save_file("Save a file.", def_path).result();

		if (!destination.empty())
		{
			path = destination;
		}

		return path;
	}

	std::filesystem::path FileSystem::folder_select_dialog(const std::string& def_path)
	{
		std::filesystem::path path {""};
		auto folder = pfd::select_folder("Select a folder.", def_path, pfd::opt::force_path).result();
		if (!folder.empty())
		{
			path = folder;
		}

		return folder;
	}
} // namespace galaxy