///
/// FileSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <execution>
#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>
#include <portable-file-dialogs.h>

#include "galaxy/error/Log.hpp"

#include "FileSystem.hpp"

// Just for this file.
namespace std_fs = std::filesystem;

namespace galaxy
{
	namespace fs
	{
		Virtual::~Virtual() noexcept
		{
			m_dirs.clear();
		}

		const bool Virtual::mount(std::string_view dir)
		{
			m_dirs.emplace_back(dir);
			if (!std_fs::is_directory(m_dirs.back()))
			{
				m_dirs.pop_back();
				return false;
			}

			return true;
		}

		const bool Virtual::mount()
		{
			return mount(show_folder_dialog());
		}

		std::string Virtual::open(std::string_view file)
		{
			const std::string path = absolute(file);
			if (path.empty())
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to open non-existant file: {0}.", file);
				return "";
			}
			else
			{
				std::ifstream ifs;
				ifs.open(path, std::ifstream::in);
				if (ifs.good())
				{
					std::stringstream buffer;
					buffer << ifs.rdbuf();
					return buffer.str();
				}
				else
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to open {0}.", file);
					return "";
				}
			}
		}

		std::vector<char> Virtual::open_binary(std::string_view file)
		{
			const std::string path = absolute(file);
			if (path.empty())
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to open non-existant file: {0}.", file);
				return {};
			}
			else
			{
				std::ifstream ifs;
				ifs.open(path, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
				if (ifs.good())
				{
					std::vector<char> buffer;

					const auto size = ifs.tellg();
					buffer.resize(size);
					ifs.seekg(0, std::ifstream::beg);
					ifs.read(&buffer[0], size);
					ifs.close();

					return buffer;
				}
				else
				{
					ifs.close();
					GALAXY_LOG(GALAXY_FATAL, "Failed to open {0}.", file);
					return {};
				}
			}
		}

		void Virtual::save(const std::string& data, std::string_view file)
		{
			auto path = absolute(file);
			if (path.empty())
			{
				path = std::filesystem::path(file).string();
			}

			std::ofstream ofs;
			ofs.open(path, std::ofstream::out | std::ofstream::trunc);
			if (ofs.good())
			{
				ofs << data;
				ofs.close();
			}
			else
			{
				ofs.close();
				GALAXY_LOG(GALAXY_FATAL, "Failed to save file data to disk: {0}.", file);
			}
		}

		void Virtual::save_binary(std::span<char> data, std::string_view file)
		{
			auto path = absolute(file);
			if (path.empty())
			{
				path = std::filesystem::path(file).string();
			}

			std::ofstream ofs;
			ofs.open(path, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
			if (ofs.good())
			{
				ofs.write(data.data(), data.size());
				ofs.close();
			}
			else
			{
				ofs.close();
				GALAXY_LOG(GALAXY_FATAL, "Failed to save file data to disk: {0}.", file);
			}
		}

		std::string Virtual::absolute(std::string_view file)
		{
			for (const auto& mounted_dir : m_dirs)
			{
				for (const auto& dir_entry : std_fs::recursive_directory_iterator(mounted_dir, std_fs::directory_options::skip_permission_denied))
				{
					const auto& path = dir_entry.path();
					if (path.filename().string() == static_cast<std::string>(file))
					{
						return std_fs::absolute(path).string();
					}
				}
			}

			return "";
		}

		std::string Virtual::show_open_dialog(const std::string& filter, const std::string& def_path)
		{
			pfd::open_file dialog {"Open file.", def_path, {"Files", filter}};
			return dialog.result().at(0);
		}

		std::string Virtual::show_save_dialog(const std::string& def_path)
		{
			pfd::save_file dialog {"Save file.", def_path};
			return dialog.result();
		}

		std::string Virtual::show_folder_dialog(const std::string& def_path)
		{
			pfd::select_folder dialog {"Select folder.", def_path, pfd::opt::force_path};
			return dialog.result();
		}
	} // namespace fs
} // namespace galaxy