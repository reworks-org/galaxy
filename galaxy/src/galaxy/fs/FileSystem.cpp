///
/// FileSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>
#include <portable-file-dialogs.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "FileSystem.hpp"

namespace galaxy
{
	namespace fs
	{
		Virtual::~Virtual() noexcept
		{
			m_dirs.clear();
		}

		void Virtual::create_file(std::string_view filepath)
		{
			const auto abs_fp = std::filesystem::absolute(filepath).string();

			std::ofstream ofs {abs_fp, std::ofstream::trunc};
			ofs.close();
		}

		std::optional<std::string> Virtual::open(std::string_view file)
		{
			const auto path = absolute(file);
			if (path == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to open non-existant file: {0}.", file);
				return std::nullopt;
			}
			else
			{
				std::ifstream ifs;
				ifs.open(path.value(), std::ifstream::in);
				if (ifs.good())
				{
					std::stringstream buffer;
					buffer << ifs.rdbuf();
					return std::make_optional(buffer.str());
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to open {0}.", file);
					return std::nullopt;
				}
			}
		}

		std::optional<std::vector<char>> Virtual::open_binary(std::string_view file)
		{
			const auto path = absolute(file);
			if (path == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to open non-existant file: {0}.", file);
				return std::nullopt;
			}
			else
			{
				std::ifstream ifs;
				ifs.open(path.value(), std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
				if (ifs.good())
				{
					std::vector<char> buffer;

					const auto size = ifs.tellg();
					buffer.resize(size);
					ifs.seekg(0, std::ifstream::beg);
					ifs.read(&buffer[0], size);
					ifs.close();

					return std::make_optional(buffer);
				}
				else
				{
					ifs.close();
					GALAXY_LOG(GALAXY_ERROR, "Failed to open {0}.", file);
					return std::nullopt;
				}
			}
		}

		const bool Virtual::save(const std::string& data, std::string_view file)
		{
			const auto path      = absolute(file);
			std::string path_str = "";
			if (path == std::nullopt)
			{
				path_str = std::filesystem::path(file).string();
			}
			else
			{
				path_str = path.value();
			}

			std::ofstream ofs;
			ofs.open(path_str, std::ofstream::out | std::ofstream::trunc);
			if (ofs.good())
			{
				ofs << data;
				ofs.close();
				return true;
			}
			else
			{
				ofs.close();
				GALAXY_LOG(GALAXY_ERROR, "Failed to save file data to disk: {0}.", file);
				return false;
			}
		}

		const bool Virtual::save_binary(std::span<char> data, std::string_view file)
		{
			const auto path      = absolute(file);
			std::string path_str = "";
			if (path == std::nullopt)
			{
				path_str = std::filesystem::path(file).string();
			}
			else
			{
				path_str = path.value();
			}

			std::ofstream ofs;
			ofs.open(path_str, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
			if (ofs.good())
			{
				ofs.write(data.data(), data.size());
				ofs.close();
				return true;
			}
			else
			{
				ofs.close();
				GALAXY_LOG(GALAXY_ERROR, "Failed to save file data to disk: {0}.", file);
				return false;
			}
		}

		std::optional<std::string> Virtual::absolute(std::string_view file)
		{
			if (std::filesystem::path(file).is_absolute())
			{
				return std::make_optional<std::string>(file);
			}
			else
			{
				const auto to_check = std::filesystem::path(file);

				for (const auto& mounted_dir : m_dirs)
				{
					for (const auto& dir_entry :
						 std::filesystem::recursive_directory_iterator(mounted_dir, std::filesystem::directory_options::skip_permission_denied))
					{
						const auto& path = dir_entry.path();

						if (path.filename() == to_check.filename())
						{
							return std::make_optional(std::filesystem::absolute(path).string());
						}
					}
				}

				return std::nullopt;
			}
		}

		std::optional<std::string> Virtual::show_open_dialog(const std::string& filter, const std::string& def_path)
		{
			pfd::open_file dialog {"Open file.", def_path, {"Files", filter}};
			auto result = dialog.result();

			if (!result.empty())
			{
				return std::make_optional(result[0]);
			}
			else
			{
				return std::nullopt;
			}
		}

		std::optional<std::string> Virtual::show_save_dialog(const std::string& filter, const std::string& def_path)
		{
			pfd::save_file dialog {"Save file.", def_path, {"Files", filter}};
			auto result = dialog.result();

			if (!result.empty())
			{
				return std::make_optional(result);
			}
			else
			{
				return std::nullopt;
			}
		}

		std::optional<std::string> Virtual::show_folder_dialog(const std::string& def_path)
		{
			pfd::select_folder dialog {"Select folder.", def_path, pfd::opt::force_path};
			auto result = dialog.result();

			if (!result.empty())
			{
				return std::make_optional(result);
			}
			else
			{
				return std::nullopt;
			}
		}

		std::optional<std::string> Virtual::open_with_dialog(const std::string& filter, const std::string& def_path)
		{
			const auto path = show_open_dialog(filter, def_path);
			if (path == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to open non-existant file.");
				return std::nullopt;
			}
			else
			{
				std::ifstream ifs;
				ifs.open(path.value(), std::ifstream::in);
				if (ifs.good())
				{
					std::stringstream buffer;
					buffer << ifs.rdbuf();
					return std::make_optional(buffer.str());
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to open {0}.", path.value());
					return std::nullopt;
				}
			}
		}

		const bool Virtual::save_with_dialog(const std::string& data, const std::string& def_path)
		{
			const auto path = show_save_dialog(def_path);
			if (path == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to save a file to an empty path.");
				return false;
			}
			else
			{
				std::ofstream ofs;
				ofs.open(path.value(), std::ofstream::out | std::ofstream::trunc);
				if (ofs.good())
				{
					ofs << data;
					ofs.close();
					return true;
				}
				else
				{
					ofs.close();
					GALAXY_LOG(GALAXY_ERROR, "Failed to save file data to disk: {0}.", path.value());
					return false;
				}
			}
		}

		const bool Virtual::mount(std::string_view dir)
		{
			if (std::filesystem::is_directory(dir))
			{
				m_dirs.emplace_back(dir);

				return true;
			}
			else
			{
				return false;
			}
		}
	} // namespace fs
} // namespace galaxy