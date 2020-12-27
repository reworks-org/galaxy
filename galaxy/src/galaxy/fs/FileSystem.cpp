///
/// FileSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <portable-file-dialogs.h>

#include "FileSystem.hpp"

namespace galaxy
{
	namespace fs
	{
		std::string open_file_dialog(const std::string& filter, const std::string& def_path)
		{
			std::string path = "";
			auto selection   = pfd::open_file("Open a file.", def_path, {"Files", filter}).result();

			if (!selection.empty())
			{
				path = std::move(selection[0]);
			}

			return path;
		}

		std::string save_file_dialog(const std::string& def_path)
		{
			std::string path = "";
			auto destination = pfd::save_file("Save a file.", def_path).result();

			if (!destination.empty())
			{
				path = std::move(destination);
			}

			return path;
		}

		std::string folder_select_dialog(const std::string& def_path)
		{
			std::string path = "";
			auto folder      = pfd::select_folder("Select a folder.", def_path, pfd::opt::force_path).result();
			if (!folder.empty())
			{
				path = std::move(folder);
			}

			return path;
		}
	} // namespace fs
} // namespace galaxy