///
/// VirtualFileSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <tinyfiledialogs.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"

#include "VirtualFileSystem.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26493)
#endif

namespace galaxy
{
	namespace fs
	{
		VirtualFileSystem::VirtualFileSystem()
		{
			auto& config = core::ServiceLocator<core::Config>::ref();

			// Create data directories.
			create_asset_layout(GALAXY_DATA_DIR, "");
			create_asset_layout(GALAXY_MOD_DIR, "");
			create_asset_layout(GALAXY_UPDATE_DIR, "");

			// Create asset directories.
			if (config.get<bool>("create_asset_work_directories"))
			{
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_MUSIC_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_SFX_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_VOICE_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_FONT_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_SCRIPT_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_SHADER_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_TEXTURE_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_ATLAS_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_LANG_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_PREFABS_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_MAPS_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_VIDEO_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_UI_DIR);
				create_asset_layout(GALAXY_WORK_DIR, GALAXY_UI_FONTS_DIR);
				create_asset_layout(GALAXY_EDITOR_DATA_DIR, "");
			}

			m_folder_type_map = {
				{   GALAXY_MUSIC_DIR,   AssetType::MUSIC},
				{     GALAXY_SFX_DIR,     AssetType::SFX},
				{   GALAXY_VOICE_DIR,   AssetType::VOICE},
				{    GALAXY_FONT_DIR,    AssetType::FONT},
				{  GALAXY_SCRIPT_DIR,  AssetType::SCRIPT},
				{  GALAXY_SHADER_DIR,  AssetType::SHADER},
				{ GALAXY_TEXTURE_DIR, AssetType::TEXTURE},
				{   GALAXY_ATLAS_DIR,   AssetType::ATLAS},
				{    GALAXY_LANG_DIR,    AssetType::LANG},
				{ GALAXY_PREFABS_DIR, AssetType::PREFABS},
				{    GALAXY_MAPS_DIR,    AssetType::MAPS},
				{   GALAXY_VIDEO_DIR,   AssetType::VIDEO},
				{      GALAXY_UI_DIR,      AssetType::UI},
				{GALAXY_UI_FONTS_DIR, AssetType::UI_FONT}
            };

			m_datapack = std::filesystem::path(GALAXY_ROOT_DIR / GALAXY_DATA_DIR / "data.galaxypak").string();
			std::replace(m_datapack.begin(), m_datapack.end(), '\\', '/');

			// Create base zip if it doesn't exist.
			if (!std::filesystem::exists(m_datapack))
			{
				zip_close(zip_open(m_datapack.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w'));
			}

			rebuild_filesystem();
		}

		VirtualFileSystem::~VirtualFileSystem()
		{
			clear();
		}

		void VirtualFileSystem::rebuild_filesystem()
		{
			clear();

			// Append base first.
			append_archive(m_datapack);

			// Then prepend updates.
			append_archives(GALAXY_UPDATE_DIR);
			append_archives(GALAXY_MOD_DIR);
		}

		void VirtualFileSystem::mkdir_disk(std::string_view path)
		{
			auto fp = std::filesystem::path(path);
			if (!fp.is_absolute())
			{
				fp = GALAXY_ROOT_DIR / path;
			}

			std::filesystem::create_directories(fp);
		}

		bool VirtualFileSystem::contains(const std::string& file)
		{
			const auto path = std::filesystem::path(file);
			return m_tree.contains(path.filename().string());
		}

		std::optional<ArchiveEntry> VirtualFileSystem::find(const std::string& file)
		{
			auto str = static_cast<std::string>(file);
			if (contains(str))
			{
				return std::make_optional(m_tree[str]);
			}

			return std::nullopt;
		}

		void VirtualFileSystem::import(const std::string& file)
		{
			auto path = std::filesystem::path(file);
			if (!path.is_absolute())
			{
				path = std::filesystem::absolute(GALAXY_ROOT_DIR / path);
			}

			if (path.string().find(GALAXY_WORK_DIR.substr(0, GALAXY_WORK_DIR.length() - 1)) != std::string::npos)
			{
				auto       fname = path.filename().string();
				const auto opt   = find(fname);
				if (opt.has_value())
				{
					remove(fname, opt.value());
				}

				auto entry = path.string();
				strutils::replace_all(entry, GALAXY_ROOT_DIR.string(), "");
				strutils::replace_all(entry, "\\", "/");
				strutils::replace_all(entry, "/" + GALAXY_WORK_DIR, "");

				auto z = zip_open(m_datapack.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'a');
				zip_entry_open(z, entry.c_str());
				zip_entry_fwrite(z, path.string().c_str());

				m_tree[fname] = ArchiveEntry {.index = zip_entry_index(z), .pack = m_datapack, .type = get_asset_type_from_path(path.string())};
				zip_entry_close(z);
				zip_close(z);

				GALAXY_LOG(GALAXY_INFO, "Imported '{0}'.", fname);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to import file outside of working directory.");
			}
		}

		void VirtualFileSystem::remove(const std::vector<std::string>& entries)
		{
			for (const auto& entry : entries)
			{
				const auto fname = std::filesystem::path(entry).filename().string();

				const auto opt = find(fname);
				if (opt.has_value())
				{
					remove(fname, opt.value());
				}
			}
		}

		void VirtualFileSystem::remove(const std::string& fname, const ArchiveEntry& entry)
		{
			if (m_tree.contains(fname))
			{
				size_t entries[] = {static_cast<size_t>(entry.index)};

				auto z = zip_open(entry.pack.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'd');
				if (zip_entries_deletebyindex(z, entries, 1) < 0)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to remove '{0}' from vfs.", fname);
				}
				zip_close(z);

				m_tree.erase(fname);
			}
		}

		std::vector<std::string> VirtualFileSystem::list_assets(const AssetType type)
		{
			std::vector<std::string> files;
			files.reserve(m_tree.size());

			std::for_each(/*std::execution::par, */ m_tree.begin(), m_tree.end(), [&](auto& pair) {
				if (pair.second.type == type)
				{
					files.emplace_back(pair.first);
				}
			});

			files.shrink_to_fit();
			return files;
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

		void VirtualFileSystem::clear()
		{
			m_tree.clear();
		}

		const robin_hood::unordered_flat_map<std::string, ArchiveEntry>& VirtualFileSystem::tree()
		{
			return m_tree;
		}

		void VirtualFileSystem::create_asset_layout(const std::filesystem::path& root, const std::string& asset_folder)
		{
			const auto merged = GALAXY_ROOT_DIR / root / asset_folder;
			if (!std::filesystem::exists(merged))
			{
				GALAXY_LOG(GALAXY_INFO, "Creating asset folder at: '{0}'.", merged.string());
				std::filesystem::create_directories(merged);
			}
		}

		void VirtualFileSystem::append_archives(const std::filesystem::path& path)
		{
			const auto di = std::filesystem::directory_iterator(GALAXY_ROOT_DIR / path, std::filesystem::directory_options::skip_permission_denied);
			for (const auto& entry : di)
			{
				if (!entry.is_directory() && entry.path().extension() == ".galaxypak")
				{
					append_archive(entry.path().string());
				}
			}
		}

		void VirtualFileSystem::append_archive(const std::string& path)
		{
			auto z = zip_open(path.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');

			for (auto i = 0; i < zip_entries_total(z); ++i)
			{
				if (zip_entry_openbyindex(z, i) >= 0)
				{
					if (!zip_entry_isdir(z))
					{
						const auto fp = std::filesystem::path(zip_entry_name(z));
						m_tree[fp.filename().string()] =
							ArchiveEntry {.index = zip_entry_index(z), .pack = path, .type = get_asset_type_from_path(fp.string())};
					}

					zip_entry_close(z);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to extract an entry from '{0}'.", path);
				}
			}

			zip_close(z);
		}

		AssetType VirtualFileSystem::get_asset_type_from_path(const std::string& path)
		{
			for (const auto& [folder, type] : m_folder_type_map)
			{
				if (path.find(folder) != std::string::npos)
				{
					return type;
				}
			}

			return AssetType::UNKNOWN;
		}
	} // namespace fs
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
