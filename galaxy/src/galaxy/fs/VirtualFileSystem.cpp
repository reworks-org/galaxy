///
/// VirtualFileSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <mimalloc.h>
#include <physfs.h>
#include <tinyfiledialogs.h>
#include <zip.h>

#include "galaxy/core/Settings.hpp"
#include "galaxy/fs/FileUtils.hpp"
#include "galaxy/logging/Log.hpp"
#include "galaxy/logging/PhysFSError.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "VirtualFileSystem.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26493)
GALAXY_DISABLE_WARNING(4244)
#endif

namespace galaxy
{
	VirtualFileSystem::VirtualFileSystem() noexcept
	{
		// Create data directories.
		if (!std::filesystem::exists(Settings::root_dir() / Settings::assets_dir()))
		{
			std::filesystem::create_directories(Settings::root_dir() / Settings::assets_dir());
		}

		// Set up allocators for physfs.
		PHYSFS_Allocator a = {};
		a.Init             = nullptr;
		a.Deinit           = nullptr;
		a.Free             = mi_free;
		a.Malloc           = mi_malloc;
		a.Realloc          = mi_realloc;
		log::physfs_check(PHYSFS_setAllocator(&a));

		// Set up vfs.
		if (log::physfs_check(PHYSFS_init(nullptr)))
		{
			PHYSFS_permitSymbolicLinks(false);

			auto write_dir = Settings::root_dir() / Settings::assets_dir();
			write_dir.make_preferred();
			log::physfs_check(PHYSFS_setWriteDir(write_dir.string().c_str()));

			auto read_dir = Settings::root_dir() / Settings::asset_pack();
			read_dir.make_preferred();

			if (Settings::use_loose_assets())
			{
				mkdir(Settings::assets_dir_music());
				mkdir(Settings::assets_dir_sfx());
				mkdir(Settings::assets_dir_voice());
				mkdir(Settings::assets_dir_font());
				mkdir(Settings::assets_dir_script());
				mkdir(Settings::assets_dir_shaders());
				mkdir(Settings::assets_dir_animation());
				mkdir(Settings::assets_dir_texture());
				mkdir(Settings::assets_dir_prefabs());
				mkdir(Settings::assets_dir_maps());
				mkdir(Settings::assets_dir_video());
				mkdir(Settings::assets_dir_ui());

				read_dir = write_dir;
			}

			log::physfs_check(PHYSFS_mount(read_dir.string().c_str(), nullptr, true));
		}
	}

	VirtualFileSystem::~VirtualFileSystem() noexcept
	{
		log::physfs_check(PHYSFS_deinit());
	}

	std::string VirtualFileSystem::read(const std::string& file)
	{
		PHYSFS_File* f = PHYSFS_openRead(file.c_str());
		if (log::physfs_check(f))
		{
			const auto len = PHYSFS_fileLength(f);

			if (log::physfs_check(len))
			{
				std::string buffer;
				buffer.resize(len);

				if (log::physfs_check(PHYSFS_readBytes(f, buffer.data(), buffer.size())))
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

			log::physfs_check(PHYSFS_close(f));
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}'.", file);
		}

		return {};
	}

	std::vector<std::uint8_t> VirtualFileSystem::read_binary(const std::string& file)
	{
		PHYSFS_File* f = PHYSFS_openRead(file.c_str());
		if (log::physfs_check(f))
		{
			const auto len = PHYSFS_fileLength(f);

			if (log::physfs_check(len))
			{
				std::vector<std::uint8_t> buffer;
				buffer.resize(len);

				if (log::physfs_check(PHYSFS_readBytes(f, &buffer[0], buffer.size())))
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

			log::physfs_check(PHYSFS_close(f));
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
		auto path = std::filesystem::path(file);
		if (path.is_absolute())
		{
			path = path.filename();
		}

		PHYSFS_File* f = PHYSFS_openWrite(path.string().c_str());
		if (log::physfs_check(f))
		{
			const auto len = PHYSFS_fileLength(f);

			if (log::physfs_check(PHYSFS_writeBytes(f, data, size)))
			{
				return true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}'.", path.string());
			}

			log::physfs_check(PHYSFS_close(f));
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}'.", path.string());
		}

		return false;
	}

	std::optional<ray::Image> VirtualFileSystem::load_ray_image(const std::string& filename) noexcept
	{
		const auto extension = fileutils::extension(filename);
		if (extension.has_value())
		{
			auto data = read_binary(filename);
			if (!data.empty())
			{
				ray::Image image = ray::LoadImageFromMemory(extension.value().c_str(), data.data(), static_cast<int>(data.size()));
				if (image.data)
				{
					return std::make_optional(image);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load {0} from memory.", filename);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read {0} from vfs.", filename);
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to read {0} extension.", filename);
		}

		return std::nullopt;
	}

	std::optional<ray::Image> VirtualFileSystem::load_ray_image_anim(const std::string& filename, int* frames) noexcept
	{
		const auto extension = fileutils::extension(filename);
		if (extension.has_value())
		{
			auto data = read_binary(filename);
			if (!data.empty())
			{
				ray::Image image = ray::LoadImageAnimFromMemory(extension.value().c_str(), data.data(), static_cast<int>(data.size()), frames);
				if (image.data)
				{
					return std::make_optional(image);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load {0} from memory.", filename);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read {0} from vfs.", filename);
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to read {0} extension.", filename);
		}

		return std::nullopt;
	}

	std::optional<ray::Texture2D> VirtualFileSystem::load_ray_texture(const std::string& filename) noexcept
	{
		const auto image = load_ray_image(filename);
		if (image.has_value())
		{
			ray::Texture2D texture = ray::LoadTextureFromImage(image.value());
			if (texture.id != 0)
			{
				ray::UnloadImage(image.value());
				return std::make_optional(texture);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load texture {0} from image.");
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to load texture {0} because of failing to read image.");
		}

		return std::nullopt;
	}

	std::optional<ray::Wave> VirtualFileSystem::load_ray_wave(const std::string& filename) noexcept
	{
		const auto extension = fileutils::extension(filename);
		if (extension.has_value())
		{
			auto data = read_binary(filename);
			if (!data.empty())
			{
				ray::Wave wave = ray::LoadWaveFromMemory(extension.value().c_str(), data.data(), static_cast<int>(data.size()));
				if (wave.data)
				{
					return std::make_optional(wave);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load {0} from memory.", filename);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read {0} from vfs.", filename);
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to read {0} extension.", filename);
		}

		return std::nullopt;
	}

	std::optional<ray::Music> VirtualFileSystem::load_ray_music_stream(const std::string& filename) noexcept
	{
		const auto extension = fileutils::extension(filename);
		if (extension.has_value())
		{
			auto data = read_binary(filename);
			if (!data.empty())
			{
				ray::Music music = ray::LoadMusicStreamFromMemory(extension.value().c_str(), data.data(), static_cast<int>(data.size()));
				if (music.ctxData)
				{
					return std::make_optional(music);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load {0} from memory.", filename);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read {0} from vfs.", filename);
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to read {0} extension.", filename);
		}

		return std::nullopt;
	}

	std::optional<ray::Font> VirtualFileSystem::load_ray_font(const std::string& filename, int font_size, std::span<int> font_chars) noexcept
	{
		const auto extension = fileutils::extension(filename);
		if (extension.has_value())
		{
			auto data = read_binary(filename);
			if (!data.empty())
			{
				ray::Font font = ray::LoadFontFromMemory(
					extension.value().c_str(),
					data.data(),
					static_cast<int>(data.size()),
					font_size,
					font_chars.data(),
					static_cast<int>(font_chars.size())
				);
				if (font.texture.id != 0)
				{
					return std::make_optional(font);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load {0} from memory.", filename);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read {0} from vfs.", filename);
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to read {0} extension.", filename);
		}

		return std::nullopt;
	}

	std::optional<ray::Shader> VirtualFileSystem::load_ray_shader(const std::string& vs_file, const std::string& fs_file) noexcept
	{
		auto vs_data = read(vs_file);
		if (vs_data.empty())
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to read vertex shader {0}.", vs_file);
		}

		auto fs_data = read(fs_file);
		if (fs_data.empty())
		{
			GALAXY_LOG(GALAXY_ERROR, "Failed to read frag shader {0}.", fs_data);
		}

		if (!vs_data.empty() && !fs_data.empty())
		{
			ray::Shader shader = ray::LoadShaderFromMemory(vs_data.data(), fs_data.data());
			if (shader.id != 0)
			{
				return std::make_optional(shader);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load shader from memory {0} | {1}.", vs_file, fs_file);
			}
		}

		return std::nullopt;
	}

	void VirtualFileSystem::mkdir(const std::string& dir) noexcept
	{
		if (!exists(dir))
		{
			log::physfs_check(PHYSFS_mkdir(dir.c_str()));
		}
	}

	void VirtualFileSystem::remove(const std::string& path) noexcept
	{
		log::physfs_check(PHYSFS_delete(path.c_str()));
	}

	bool VirtualFileSystem::exists(const std::string& file) noexcept
	{
		return PHYSFS_exists(file.c_str());
	}

	bool VirtualFileSystem::is_dir(const std::string& path) noexcept
	{
		return PHYSFS_isDirectory(path.c_str());
	}

	std::vector<std::string> VirtualFileSystem::list(const std::string& dir)
	{
		std::vector<std::string> list;

		log::physfs_check(PHYSFS_enumerate(
			dir.c_str(),
			[](void* data, const char* origdir, const char* fname) -> PHYSFS_EnumerateCallbackResult {
				if (data != nullptr && fname != nullptr)
				{
					std::string o = origdir;
					std::string f = fname;

					auto* my_list = static_cast<std::vector<std::string>*>(data);
					my_list->emplace_back(o + f);
				}

				return PHYSFS_ENUM_OK;
			},
			&list
		));

		return list;
	}

	void VirtualFileSystem::alert() noexcept
	{
		tinyfd_beep();
	}

	void VirtualFileSystem::notification(const std::string& title, const std::string& msg, const DialogIcon icon) noexcept
	{
		std::string tinyfd_icon {magic_enum::enum_name(icon)};
		tinyfd_notifyPopup(title.c_str(), msg.c_str(), tinyfd_icon.c_str());
	}

	int VirtualFileSystem::message_box(const std::string& title, const std::string& msg, const DialogType type, const DialogIcon icon, const DialogButton btn) noexcept
	{
		std::string tinyfd_type {magic_enum::enum_name(type)};
		std::string tinyfd_icon {magic_enum::enum_name(icon)};

		return tinyfd_messageBox(title.c_str(), msg.c_str(), tinyfd_type.c_str(), tinyfd_icon.c_str(), static_cast<int>(btn));
	}

	std::string VirtualFileSystem::input_box(const std::string& title, const std::string& msg, const std::string& default_text, const bool password) noexcept
	{
		const char* dt = password ? nullptr : default_text.c_str();
		return tinyfd_inputBox(title.c_str(), msg.c_str(), dt);
	}

	std::string VirtualFileSystem::open_save_dialog(const std::string& default_filename, const std::vector<const char*>& filters)
	{
		const char* const* filter_patterns = (filters.size() > 0) ? filters.data() : nullptr;
		const char*        result          = tinyfd_saveFileDialog("Save file", default_filename.c_str(), static_cast<int>(filters.size()), filter_patterns, nullptr);

		if (result != nullptr)
		{
			return {result};
		}
		else
		{
			return {};
		}
	}

	std::string VirtualFileSystem::open_file_dialog(const std::vector<const char*>& filters, const std::string& def_path)
	{
		const auto default_path = (Settings::root_dir() / def_path).string();

		const char* const* filter_patterns = (filters.size() > 0) ? filters.data() : nullptr;
		const char*        result          = tinyfd_openFileDialog("Open file", default_path.c_str(), static_cast<int>(filters.size()), filter_patterns, "Select a file", false);

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
		const auto  default_path = (Settings::root_dir() / def_path).string();
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
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
