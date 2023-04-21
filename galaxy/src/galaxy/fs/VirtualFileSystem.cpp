///
/// VirtualFileSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <magic_enum.hpp>
#include <tinyfiledialogs.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/utils/StringUtils.hpp"

#include "VirtualFileSystem.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(4267)
#endif

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
						info.code   = FileCode::FOUND;
						info.path   = path;
						info.string = path.string();

						return info;
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Provided path is not located in the vfs: {0}.", path.string());

						info.code = FileCode::NOT_IN_VFS;
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

					const auto directory_iterator =
						std::filesystem::recursive_directory_iterator(start_dir, std::filesystem::directory_options::skip_permission_denied);

					for (const auto& dir_entry : directory_iterator)
					{
						const auto& dir_path = dir_entry.path();

						if (path.filename() == dir_path.filename())
						{
							const auto abs_dir_path = std::filesystem::absolute(dir_path);

							info.code   = FileCode::FOUND;
							info.path   = abs_dir_path;
							info.string = abs_dir_path.string();

							return info;
						}
					}

					info.code   = FileCode::NOT_FOUND;
					info.path   = m_root / path;
					info.string = info.path.string();

					return info;
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "VFS::Find() cannot be used to search for a folder.");

				info.code = FileCode::NO_EXTENSION;
				return info;
			}
		}

		bool VirtualFileSystem::create_file(std::string_view path, const bool binary)
		{
			const auto found = find(path);

			if (found.code == FileCode::NOT_FOUND)
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
			else if (found.code == FileCode::FOUND)
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

		std::string VirtualFileSystem::open(std::string_view file)
		{
			if (!file.empty())
			{
				const auto path = find(file);

				if (path.code == FileCode::FOUND)
				{
					std::ifstream ifs {path.string, std::ifstream::in};

					if (ifs.good())
					{
						std::stringstream buffer;
						buffer << ifs.rdbuf();
						ifs.close();

						return buffer.str();
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to open file: {0}.", file);
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Passed empty string to VFS::Open");
			}

			return {};
		}

		meta::vector<char> VirtualFileSystem::open_binary(std::string_view file)
		{
			if (!file.empty())
			{
				const auto path = find(file);

				if (path.code == FileCode::FOUND)
				{
					std::ifstream ifs {path.string, std::ifstream::in | std::ifstream::binary | std::ifstream::ate};

					if (ifs.good())
					{
						meta::vector<char> buffer;

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

						GALAXY_LOG(GALAXY_ERROR, "Failed to open binary: {0}.", file);
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Passed empty string to VFS::Open");
			}

			return {};
		}

		bool VirtualFileSystem::save(const std::string& data, std::string_view file)
		{
			if (!data.empty() && !file.empty())
			{
				const auto path = find(file);

				if (path.code == FileCode::FOUND || path.code == FileCode::NOT_FOUND)
				{
					std::ofstream ofs {path.string, std::ofstream::out | std::ofstream::trunc};

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
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Passed empty data or filename to VFS::save");
			}

			return false;
		}

		bool VirtualFileSystem::save_binary(std::span<char> data, std::string_view file)
		{
			if (!data.empty() && !file.empty())
			{
				const auto path = find(file);

				if (path.code == FileCode::FOUND || path.code == FileCode::NOT_FOUND)
				{
					std::ofstream ofs {path.string, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary};

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
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Passed empty data or filename to VFS::save_binary");
			}

			return false;
		}

		bool VirtualFileSystem::exists(std::string_view file)
		{
			const auto info = find(file);
			return info.code == FileCode::FOUND;
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

		void VirtualFileSystem::alert()
		{
			tinyfd_beep();
		}

		void VirtualFileSystem::trigger_notification(const std::string& title, const std::string& msg, const DialogIcon icon)
		{
			std::string tinyfd_icon {magic_enum::enum_name(icon)};
			tinyfd_notifyPopup(title.c_str(), msg.c_str(), tinyfd_icon.c_str());
		}

		std::string VirtualFileSystem::open_file_dialog(const meta::vector<const char*>& filters, const std::string& def_path)
		{
			const auto default_path = (m_root / def_path).string();

			const char* const* filter_patterns = (filters.size() > 0) ? filters.data() : nullptr;
			const char* result = tinyfd_openFileDialog("Open file.", default_path.c_str(), filters.size(), filter_patterns, "Select a file.", false);

			if (result != nullptr)
			{
				return {result};
			}
			else
			{
				return {};
			}
		}

		meta::vector<std::string> VirtualFileSystem::open_file_dialog_multi(const meta::vector<const char*>& filters, const std::string& def_path)
		{
			const auto default_path = (m_root / def_path).string();

			const char* const* filter_patterns = (filters.size() > 0) ? filters.data() : nullptr;
			const char* result = tinyfd_openFileDialog("Open file.", default_path.c_str(), filters.size(), filter_patterns, "Select a file.", false);

			if (result != nullptr)
			{
				return strutils::split(result, "|");
			}
			else
			{
				return {};
			}
		}

		std::string VirtualFileSystem::open_save_dialog(const std::string& default_filename, const meta::vector<const char*>& filters)
		{
			const char* const* filter_patterns = (filters.size() > 0) ? filters.data() : nullptr;
			const char* result                 = tinyfd_saveFileDialog("Save file.", default_filename.c_str(), filters.size(), filter_patterns, nullptr);

			if (result != nullptr)
			{
				return {result};
			}
			else
			{
				return {};
			}
		}

		std::string VirtualFileSystem::select_folder_dialog(const std::string& def_path)
		{
			const auto default_path = (m_root / def_path).string();
			const char* result      = tinyfd_selectFolderDialog("Select folder.", default_path.c_str());

			if (result != nullptr)
			{
				return {result};
			}
			else
			{
				return {};
			}
		}

		std::string VirtualFileSystem::open_using_dialog(const meta::vector<const char*>& filters, const std::string& def_path)
		{
			const auto path = open_file_dialog(filters, def_path);
			return open(path);
		}

		meta::vector<char> VirtualFileSystem::open_binary_using_dialog(const meta::vector<const char*>& filters, const std::string& def_path)
		{
			const auto path = open_file_dialog(filters, def_path);
			return open_binary(path);
		}

		bool VirtualFileSystem::save_using_dialog(const std::string& data, const std::string& default_filename, const meta::vector<const char*>& filters)
		{
			const auto path = open_save_dialog(default_filename, filters);
			return save(data, path);
		}

		bool VirtualFileSystem::save_binary_using_dialog(std::span<char> data, const std::string& default_filename, const meta::vector<const char*>& filters)
		{
			const auto path = open_save_dialog(default_filename, filters);
			return save_binary(data, path);
		}

		meta::vector<std::string> VirtualFileSystem::list_directory(std::string_view path)
		{
			meta::vector<std::string> output;

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

		std::string VirtualFileSystem::root()
		{
			return m_root.string();
		}

		const std::filesystem::path& VirtualFileSystem::root_path() const
		{
			return m_root;
		}
	} // namespace fs
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif