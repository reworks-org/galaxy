///
/// VirtualFileSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <portable-file-dialogs.h>

#include "galaxy/error/Log.hpp"

#include "VirtualFileSystem.hpp"

namespace galaxy
{
	namespace fs
	{
		VirtualFileSystem::VirtualFileSystem(std::string_view root)
			: m_root {root}
		{
			if (std::filesystem::path(m_root).has_extension())
			{
				GALAXY_LOG(GALAXY_FATAL, "Cannot have a file as root directory for vfs.");
			}
			else
			{
				if (!m_root.is_absolute())
				{
					m_root = std::filesystem::absolute(std::filesystem::current_path() / std::filesystem::path(root)).string();
				}
			}
		}

		FileInfo VirtualFileSystem::find(std::string_view file)
		{
			FileInfo info;
			const auto path = std::filesystem::path(file);

			if (path.has_extension())
			{
				if (path.is_absolute())
				{
					if (path.string().find(m_root.string()) != std::string::npos)
					{
						info.m_code   = FileCode::FOUND;
						info.m_path   = path;
						info.m_string = path.string();

						return info;
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Provided path is not located in the vfs: {0}.", path.string());

						info.m_code = FileCode::NOT_IN_VFS;
						return info;
					}
				}
				else
				{
					auto start_dir = m_root.string();
					if (path.has_parent_path())
					{
						start_dir += path.parent_path().string();
					}

					for (const auto& dir_entry :
						std::filesystem::recursive_directory_iterator(start_dir, std::filesystem::directory_options::skip_permission_denied))
					{
						const auto& dir_path = dir_entry.path();

						if (path.filename() == dir_path.filename())
						{
							const auto abs_dir_path = std::filesystem::absolute(dir_path);

							info.m_code   = FileCode::FOUND;
							info.m_path   = abs_dir_path;
							info.m_string = abs_dir_path.string();

							return info;
						}
					}

					info.m_code   = FileCode::NOT_FOUND;
					info.m_path   = m_root / path;
					info.m_string = info.m_path.string();

					return info;
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "VFS::Find() cannot be used to search for a folder.");

				info.m_code = FileCode::NO_EXTENSION;
				return info;
			}
		}

		bool VirtualFileSystem::create_file(std::string_view path, const bool binary)
		{
			const auto found = find(path);

			if (found.m_code == FileCode::NOT_FOUND)
			{
				std::ofstream::openmode flags = std::ofstream::trunc;

				if (binary)
				{
					flags |= std::ofstream::binary;
				}

				const auto fs_path = m_root / std::filesystem::path(path);

				std::ofstream ofs {fs_path.string(), flags};
				ofs.close();

				return true;
			}
			else if (found.m_code == FileCode::FOUND)
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to create file that already exists: {0}.", path);
				return false;
			}
			else
			{
				return false;
			}
		}

		bool VirtualFileSystem::create_folder(std::string_view path)
		{
			bool result = true;

			auto fs_path = std::filesystem::path(path);

			if (!fs_path.has_extension())
			{
				if (fs_path.is_absolute())
				{
					if (fs_path.string().find(m_root.string()) != std::string::npos)
					{
						if (!std::filesystem::exists(fs_path))
						{
							// Create folders.
							std::filesystem::create_directories(fs_path);
						}
						else
						{
							GALAXY_LOG(GALAXY_ERROR, "Folder: {0} already exists.", path);
							result = false;
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Provided path is not located in the vfs: {0}.", path);
						result = false;
					}
				}
				else
				{
					fs_path = m_root / std::filesystem::path(path);
					if (!std::filesystem::exists(fs_path))
					{
						// Create folders.
						std::filesystem::create_directories(fs_path);
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Folder: {0} already exists.", path);
						result = false;
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to create a folder: {0}, that is actually a file.", path);
				result = false;
			}

			return result;
		}

		std::optional<std::string> VirtualFileSystem::open(std::string_view file)
		{
			const auto path = find(file);

			if (path.m_code == FileCode::FOUND)
			{
				std::ifstream ifs {path.m_string, std::ifstream::in};

				if (ifs.good())
				{
					std::stringstream buffer;
					buffer << ifs.rdbuf();
					ifs.close();

					return std::make_optional(buffer.str());
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to open file: {0}.", file);
					return std::nullopt;
				}
			}
			else
			{
				return std::nullopt;
			}
		}

		std::optional<std::vector<char>> VirtualFileSystem::open_binary(std::string_view file)
		{
			const auto path = find(file);

			if (path.m_code == FileCode::FOUND)
			{
				std::ifstream ifs {path.m_string, std::ifstream::in | std::ifstream::binary | std::ifstream::ate};

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

					GALAXY_LOG(GALAXY_ERROR, "Failed to open binary: {0}.", file);
					return std::nullopt;
				}
			}
			else
			{
				return std::nullopt;
			}
		}

		bool VirtualFileSystem::save(const std::string& data, std::string_view file)
		{
			const auto path = find(file);

			if (path.m_code == FileCode::FOUND || path.m_code == FileCode::NOT_FOUND)
			{
				std::ofstream ofs {path.m_string, std::ofstream::out | std::ofstream::trunc};

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
			else
			{
				return false;
			}
		}

		bool VirtualFileSystem::save_binary(std::span<char> data, std::string_view file)
		{
			const auto path = find(file);

			if (path.m_code == FileCode::FOUND || path.m_code == FileCode::NOT_FOUND)
			{
				std::ofstream ofs {path.m_string, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary};

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
			else
			{
				return false;
			}
		}

		void VirtualFileSystem::remove(std::string_view path)
		{
			auto fs_path = std::filesystem::path(path);

			if (!fs_path.is_absolute())
			{
				fs_path = m_root / fs_path;
			}
			else
			{
				if (fs_path.string().find(m_root.string()) == std::string::npos)
				{
					GALAXY_LOG(GALAXY_ERROR, "Path to remove is not located in the vfs: {0}.", path);
					return;
				}
			}

			if (std::filesystem::exists(fs_path))
			{
				std::filesystem::remove(fs_path);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to remove path that does not exist: {0}.", path);
			}
		}

		std::optional<std::string> VirtualFileSystem::show_open_dialog(const std::string& filter, const std::string& def_path)
		{
			std::filesystem::path default_path = m_root;

			if (!def_path.empty())
			{
				default_path /= def_path;
			}

			pfd::open_file dialog {"Open file.", default_path.string(), {"Select file", filter}};

			const auto result = dialog.result();
			if (!result.empty())
			{
				return std::make_optional(result[0]);
			}
			else
			{
				return std::nullopt;
			}
		}

		std::optional<std::vector<std::string>> VirtualFileSystem::show_open_dialog_list(const std::string& filter, const std::string& def_path)
		{
			std::filesystem::path default_path = m_root;

			if (!def_path.empty())
			{
				default_path /= def_path;
			}

			pfd::open_file dialog {"Open file.", default_path.string(), {"Select multiple files", filter}, pfd::opt::multiselect};

			const auto result = dialog.result();
			if (!result.empty())
			{
				return std::make_optional(result);
			}
			else
			{
				return std::nullopt;
			}
		}

		std::optional<std::string> VirtualFileSystem::show_save_dialog(const std::string& filter, const std::string& def_path)
		{
			std::filesystem::path default_path = m_root;

			if (!def_path.empty())
			{
				default_path /= def_path;
			}

			pfd::save_file dialog {"Save file.", default_path.string(), {"Select file", filter}};

			const auto result = dialog.result();
			if (!result.empty())
			{
				return std::make_optional(result);
			}
			else
			{
				return std::nullopt;
			}
		}

		std::optional<std::string> VirtualFileSystem::show_folder_dialog(const std::string& def_path)
		{
			std::filesystem::path default_path = m_root;

			if (!def_path.empty())
			{
				default_path /= def_path;
			}

			pfd::select_folder dialog {"Select folder.", default_path.string(), pfd::opt::force_path};

			const auto result = dialog.result();
			if (!result.empty())
			{
				return std::make_optional(result);
			}
			else
			{
				return std::nullopt;
			}
		}

		std::optional<std::string> VirtualFileSystem::open_with_dialog(const std::string& filter, const std::string& def_path)
		{
			const auto path = show_open_dialog(filter, def_path);
			if (path.has_value())
			{
				return open(path.value());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to open non-existant file.");
				return std::nullopt;
			}
		}

		bool VirtualFileSystem::save_with_dialog(const std::string& data, const std::string& def_path)
		{
			const auto path = show_save_dialog(def_path);
			if (path.has_value())
			{
				return save(data, path.value());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to save a file to an empty path.");
				return false;
			}
		}

		std::vector<std::string> VirtualFileSystem::list_directory(std::string_view path)
		{
			std::vector<std::string> output;

			auto fs_path = std::filesystem::path(path);
			if (!fs_path.is_absolute())
			{
				fs_path = m_root / fs_path;
			}

			if (std::filesystem::is_directory(fs_path))
			{
				if (fs_path.string().find(m_root.string()) != std::string::npos)
				{
					if (std::filesystem::exists(fs_path))
					{
						for (const auto& dir_entry :
							std::filesystem::recursive_directory_iterator(fs_path, std::filesystem::directory_options::skip_permission_denied))
						{
							output.emplace_back(dir_entry.path().string());
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Attempted to list a folder that does not exist: '{0}'", path);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Path to list is not located in the vfs: '{0}'", path);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to list the contents of a non-directory: '{0}'", path);
			}

			return output;
		}

		std::string VirtualFileSystem::root() noexcept
		{
			return m_root.string();
		}

		const std::filesystem::path& VirtualFileSystem::root_path() const noexcept
		{
			return m_root;
		}
	} // namespace fs
} // namespace galaxy