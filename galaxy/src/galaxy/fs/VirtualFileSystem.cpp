///
/// VirtualFileSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <mimalloc.h>
#include <physfs.h>
#include <tinyfiledialogs.h>
#include <zip.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/error/PhysFSError.hpp"

#include "VirtualFileSystem.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26493)
GALAXY_DISABLE_WARNING(4244)
#endif

namespace galaxy
{
	namespace fs
	{
		VirtualFileSystem::VirtualFileSystem()
		{
			// Create data directories.
			std::filesystem::create_directories(GALAXY_ROOT_DIR / GALAXY_ASSET_DIR);
			std::filesystem::create_directories(GALAXY_ROOT_DIR / GALAXY_EDITOR_DATA_DIR);

			// Set up allocators for physfs.
			PHYSFS_Allocator a = {};
			a.Init             = nullptr;
			a.Deinit           = nullptr;
			a.Free             = mi_free;
			a.Malloc           = mi_malloc;
			a.Realloc          = mi_realloc;
			error::physfs_check(PHYSFS_setAllocator(&a));

			// Set up vfs.
			if (error::physfs_check(PHYSFS_init(nullptr)))
			{
				PHYSFS_permitSymbolicLinks(false);

				auto write_dir = GALAXY_ROOT_DIR / GALAXY_ASSET_DIR;
				write_dir.make_preferred();
				error::physfs_check(PHYSFS_setWriteDir(write_dir.string().c_str()));

				auto& config   = core::ServiceLocator<core::Config>::ref();
				auto  read_dir = GALAXY_ROOT_DIR / config.get<std::string>("asset_pak");
				read_dir.make_preferred();

				// Create asset pack if it doesn't exist.
				if (!std::filesystem::exists(read_dir))
				{
					zip_close(zip_open(read_dir.string().c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w'));
				}

				if (config.get<bool>("use_loose_assets"))
				{
					read_dir = write_dir;
				}

				error::physfs_check(PHYSFS_mount(read_dir.string().c_str(), nullptr, true));

				const auto merged = GALAXY_ROOT_DIR / GALAXY_EDITOR_DATA_DIR;
				error::physfs_check(PHYSFS_mount(merged.string().c_str(), nullptr, true));

				mkdir(GALAXY_MUSIC_DIR);
				mkdir(GALAXY_SFX_DIR);
				mkdir(GALAXY_VOICE_DIR);
				mkdir(GALAXY_FONT_DIR);
				mkdir(GALAXY_SCRIPT_DIR);
				mkdir(GALAXY_SHADER_DIR);
				mkdir(GALAXY_TEXTURE_DIR);
				mkdir(GALAXY_ATLAS_DIR);
				mkdir(GALAXY_LANG_DIR);
				mkdir(GALAXY_PREFABS_DIR);
				mkdir(GALAXY_MAPS_DIR);
				mkdir(GALAXY_VIDEO_DIR);
				mkdir(GALAXY_UI_DIR);
				mkdir(GALAXY_UI_FONTS_DIR);
			}
		}

		VirtualFileSystem::~VirtualFileSystem()
		{
			error::physfs_check(PHYSFS_deinit());
		}

		std::string VirtualFileSystem::read(const std::string& file)
		{
			PHYSFS_File* f = PHYSFS_openRead(file.c_str());
			if (error::physfs_check(f))
			{
				const auto len = PHYSFS_fileLength(f);

				if (error::physfs_check(len))
				{
					std::string buffer;
					buffer.resize(len);

					if (error::physfs_check(PHYSFS_readBytes(f, buffer.data(), buffer.size())))
					{
						return buffer;
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}'.", file);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to check file length for '{0}'.", file);
				}

				error::physfs_check(PHYSFS_close(f));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}'.", file);
			}

			return {};
		}

		meta::vector<std::uint8_t> VirtualFileSystem::read_binary(const std::string& file)
		{
			PHYSFS_File* f = PHYSFS_openRead(file.c_str());
			if (error::physfs_check(f))
			{
				const auto len = PHYSFS_fileLength(f);

				if (error::physfs_check(len))
				{
					meta::vector<std::uint8_t> buffer;
					buffer.resize(len);

					if (error::physfs_check(PHYSFS_readBytes(f, &buffer[0], buffer.size())))
					{
						return buffer;
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}'.", file);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to check file length for '{0}'.", file);
				}

				error::physfs_check(PHYSFS_close(f));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}'.", file);
			}

			return {};
		}

		bool VirtualFileSystem::write(const std::string& data, const std::string& file)
		{
			return write_raw(data.data(), data.size() * sizeof(char), file);
		}

		bool VirtualFileSystem::write_binary(std::span<std::uint8_t> data, const std::string& file)
		{
			return write_raw(data.data(), data.size_bytes(), file);
		}

		bool VirtualFileSystem::write_raw(const void* data, const std::size_t size, const std::string& file)
		{
			PHYSFS_File* f = PHYSFS_openWrite(file.c_str());
			if (error::physfs_check(f))
			{
				const auto len = PHYSFS_fileLength(f);

				if (error::physfs_check(PHYSFS_writeBytes(f, data, size)))
				{
					return true;
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}'.", file);
				}

				error::physfs_check(PHYSFS_close(f));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}'.", file);
			}

			return false;
		}

		void VirtualFileSystem::mkdir(const std::string& dir)
		{
			if (!exists(dir))
			{
				error::physfs_check(PHYSFS_mkdir(dir.c_str()));
			}
		}

		void VirtualFileSystem::remove(const std::string& path)
		{
			error::physfs_check(PHYSFS_delete(path.c_str()));
		}

		bool VirtualFileSystem::exists(const std::string& file)
		{
			return PHYSFS_exists(file.c_str());
		}

		bool VirtualFileSystem::is_dir(const std::string& path)
		{
			return PHYSFS_isDirectory(path.c_str());
		}

		meta::vector<std::string> VirtualFileSystem::list(const std::string& dir)
		{
			meta::vector<std::string> list;

			error::physfs_check(PHYSFS_enumerate(
				dir.c_str(),
				[](void* data, const char* origdir, const char* fname) -> PHYSFS_EnumerateCallbackResult {
					if (data != nullptr && fname != nullptr)
					{
						std::string o = origdir;
						std::string f = fname;

						auto* my_list = static_cast<meta::vector<std::string>*>(data);
						my_list->emplace_back(o + f);
					}

					return PHYSFS_ENUM_OK;
				},
				&list));

			return list;
		}

		void VirtualFileSystem::alert()
		{
			tinyfd_beep();
		}

		void VirtualFileSystem::notification(const std::string& title, const std::string& msg, const DialogIcon icon)
		{
			std::string tinyfd_icon {magic_enum::enum_name(icon)};
			tinyfd_notifyPopup(title.c_str(), msg.c_str(), tinyfd_icon.c_str());
		}

		int
		VirtualFileSystem::message_box(const std::string& title, const std::string& msg, const DialogType type, const DialogIcon icon, const DialogButton btn)
		{
			std::string tinyfd_type {magic_enum::enum_name(type)};
			std::string tinyfd_icon {magic_enum::enum_name(icon)};

			return tinyfd_messageBox(title.c_str(), msg.c_str(), tinyfd_type.c_str(), tinyfd_icon.c_str(), static_cast<int>(btn));
		}

		std::string VirtualFileSystem::input_box(const std::string& title, const std::string& msg, const std::string& default_text, const bool password)
		{
			const char* dt = password ? nullptr : default_text.c_str();
			return tinyfd_inputBox(title.c_str(), msg.c_str(), dt);
		}

		std::string VirtualFileSystem::open_save_dialog(const std::string& default_filename, const meta::vector<const char*>& filters)
		{
			const char* const* filter_patterns = (filters.size() > 0) ? filters.data() : nullptr;
			const char* result = tinyfd_saveFileDialog("Save file", default_filename.c_str(), static_cast<int>(filters.size()), filter_patterns, nullptr);

			if (result != nullptr)
			{
				return {result};
			}
			else
			{
				return {};
			}
		}

		std::string VirtualFileSystem::open_file_dialog(const meta::vector<const char*>& filters, const std::string& def_path)
		{
			const auto default_path = (GALAXY_ROOT_DIR / def_path).string();

			const char* const* filter_patterns = (filters.size() > 0) ? filters.data() : nullptr;
			const char*        result =
				tinyfd_openFileDialog("Open file", default_path.c_str(), static_cast<int>(filters.size()), filter_patterns, "Select a file", false);

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
			const auto  default_path = (GALAXY_ROOT_DIR / def_path).string();
			const char* result       = tinyfd_selectFolderDialog("Select folder", default_path.c_str());

			if (result != nullptr)
			{
				return {result};
			}
			else
			{
				return {};
			}
		}
	} // namespace fs
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
