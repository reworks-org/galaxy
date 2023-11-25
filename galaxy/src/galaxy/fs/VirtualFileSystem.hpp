///
/// VirtualFileSystem.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_VIRTUALFILESYSTEM_HPP_
#define GALAXY_FS_VIRTUALFILESYSTEM_HPP_

#include <fstream>

#include <zip.h>

#include "galaxy/fs/ArchiveEntry.hpp"
#include "galaxy/fs/DialogButton.hpp"
#include "galaxy/fs/DialogIcons.hpp"
#include "galaxy/fs/DialogType.hpp"
#include "galaxy/meta/FileType.hpp"

namespace galaxy
{
	namespace fs
	{
		///
		/// File System to make managing files easier.
		///
		class VirtualFileSystem final
		{
		  public:
			///
			/// Constructor.
			///
			VirtualFileSystem();

			///
			/// Destructor.
			///
			~VirtualFileSystem();

			///
			/// Rebuild the filesystem index cache.
			///
			void rebuild_filesystem();

			///
			/// \brief Creates an empty folder in the filesystem relative to the root.
			///
			/// This writes to the root directory, not the archives.
			///
			/// \param path Name of folder to create. Can contain a path. I.e. "assets/dir". Must end in a '/'.
			///
			void mkdir_disk(std::string_view path);

			///
			/// Does the VFS contain the file.
			///
			/// \param file File to check.
			///
			/// \return True if file exists.
			///
			[[nodiscard]] bool contains(const std::string& file);

			///
			/// Find an entry in the vfs.
			///
			/// \param file File to get.
			///
			/// \return Entry in VFS. Nullopt if it couldn't be found.
			///
			[[nodiscard]] std::optional<ArchiveEntry> find(const std::string& file);

			///
			/// Read a file from the vfs.
			///
			/// \tparam FileType Type of file to read.
			///
			/// \param data Data to write to file.
			/// \param file File to write to.
			///
			/// \return Buffer containing read data.
			///
			template<meta::vfs_read_type FileType>
			[[nodiscard]] FileType read(const std::string& file);

			///
			/// \brief Writes a file to the vfs.
			///
			/// Checks for existing file to overwrite first.
			///
			/// \tparam FileType Type of file to write.
			///
			/// \param data Data to write to file.
			/// \param file File to write to.
			///
			/// \return True if successful.
			///
			template<meta::vfs_write_type FileType>
			[[nodiscard]] bool write(FileType data, const std::string& file);

			///
			/// Read a file from disk.
			///
			/// \tparam FileType Type of file to read.
			///
			/// \param file File to read from. Must be in current_dir / assets /	...
			///
			/// \return Buffer containing read data.
			///
			template<meta::vfs_read_type FileType>
			[[nodiscard]] FileType read_disk(const std::string& file);

			///
			/// \brief Writes a file to disk.
			///
			/// Checks for existing file to overwrite first.
			///
			/// \tparam FileType Type of file to write.
			///
			/// \param data Data to write to file.
			/// \param file File to write to. Must be in current_dir / assets /	...
			///
			/// \return True if successful.
			///
			template<meta::vfs_write_type FileType>
			[[nodiscard]] bool write_disk(FileType data, const std::string& file);

			///
			/// \brief Imports a file from the working directory to the archive.
			///
			/// If file includes path, these paths are also written into the archive.
			///
			/// \param file File in the working directory to import.
			///
			void import(const std::string& file);

			///
			/// \brief Delete file(s) or folder(s).
			///
			/// Does not delete directories in path.
			/// Does nothing if entry does not exist.
			///
			/// \param path Entry(s) to delete.
			///
			void remove(const std::vector<std::string>& entries);

			///
			/// Removes a file from an archive by the entry.
			///
			/// \param fname Name of the file in the tree.
			/// \param entry Entry to remove.
			///
			void remove(const std::string& fname, const ArchiveEntry& entry);

			///
			/// Get a list of all assets of a specific type.
			///
			/// \param Type of asset to list out.
			///
			/// \return List of asset entry names.
			///
			std::vector<std::string> list_assets(const AssetType type);

			///
			/// Trigger a standard filesystem audio alert.
			///
			void alert();

			///
			/// Trigger a system notification.
			///
			/// \param title Notification title.
			/// \param msg Body text of notification.
			/// \param icon Icon to display alongside text.
			///
			void notification(const std::string& title, const std::string& msg, const DialogIcon icon);

			///
			/// Opens a platform specific message box.
			///
			/// \param title Notification title.
			/// \param msg Body text of notification.
			/// \param type Type of message box.
			/// \param icon Icon to display alongside text.
			/// \param btn Button to show on message box.
			///
			/// \return Integer with outcome. 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel.
			///
			[[nodiscard]] int
			message_box(const std::string& title, const std::string& msg, const DialogType type, const DialogIcon icon, const DialogButton btn);

			///
			/// Opens a platform specific text input box.
			///
			/// \param title Notification title.
			/// \param msg Body text of notification.
			/// \param default_text Default text displayed in input box.
			/// \param password True to obscure input. Note that enabling this disables default_text.
			///
			/// \return Input as a string.
			///
			[[nodiscard]] std::string
			input_box(const std::string& title, const std::string& msg, const std::string& default_text = "", const bool password = false);

			///
			/// Open a save file dialog.
			///
			/// \param default_filename Default name to save file as.
			/// \param filters List of filters for dialog, i.e. "*.png", "*.jpg", etc.
			///
			/// \return String with path, empty if error.
			///
			[[nodiscard]] std::string open_save_dialog(const std::string& default_filename, const meta::vector<const char*>& filters = {});

			///
			/// Open a file dialog.
			///
			/// \param filters List of filters for dialog, i.e. "*.png", "*.jpg", etc.
			/// \param def_path Default starting path to open dialog at.
			///
			/// \return String with path, empty if error.
			///
			[[nodiscard]] std::string open_file_dialog(const meta::vector<const char*>& filters = {}, const std::string& def_path = "");

			///
			/// Select a folder using a dialog.
			///
			/// \param def_path Default starting path to open dialog at.
			///
			/// \return String with path, empty if error.
			///
			[[nodiscard]] std::string select_folder_dialog(const std::string& def_path = "");

			///
			/// Close all handles except root archive, and clear directory tree.
			///
			void clear();

			///
			/// Get filesystem tree.
			///
			/// \return tree Read-only access to underlying file system tree.
			///
			const robin_hood::unordered_flat_map<std::string, ArchiveEntry>& tree();

		  private:
			///
			/// Copy constructor.
			///
			VirtualFileSystem(const VirtualFileSystem&) = delete;

			///
			/// Move constructor.
			///
			VirtualFileSystem(VirtualFileSystem&&) = delete;

			///
			/// Copy assignment operator.
			///
			VirtualFileSystem& operator=(const VirtualFileSystem&) = delete;

			///
			/// Move assignment operator.
			///
			VirtualFileSystem& operator=(VirtualFileSystem&&) = delete;

			///
			/// \brief Create default asset layout.
			///
			/// This uses a const string reference over string_view since
			/// we need to combine the strings into a new string.
			///
			/// \param root Root asset folder.
			/// \param asset_folder Path to the asset folder to create.
			///
			void create_asset_layout(const std::filesystem::path& root, const std::string& asset_folder);

			///
			/// Append archives within a directory to the search path.
			///
			/// \param path Directory path.
			///
			void append_archives(const std::filesystem::path& path);

			///
			/// Append archive entries to the tree.
			///
			/// \param path Directory path.
			///
			void append_archive(const std::string& path);

			///
			/// Converts a path to a specific asset type.
			///
			/// \param path Path to check and convert.
			///
			/// \return Type of asset that belongs in that path.
			///
			AssetType get_asset_type_from_path(const std::string& path);

		  private:
			///
			/// Path to main data.
			///
			std::string m_datapack;

			///
			/// Other entries in the archive field.
			///
			robin_hood::unordered_flat_map<std::string, ArchiveEntry> m_tree;

			///
			/// Folder <-> Type mapping.
			///
			robin_hood::unordered_flat_map<std::string, AssetType> m_folder_type_map;
		};

		template<meta::vfs_read_type FileType>
		inline FileType VirtualFileSystem::read(const std::string& file)
		{
			auto entry_opt = find(file);
			if (entry_opt.has_value())
			{
				FileType buffer {};

				auto& entry = entry_opt.value();

				auto z = zip_open(entry.pack.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');

				if (zip_entry_open(z, entry.entry.c_str()) >= 0)
				{
					buffer.resize(zip_entry_size(z));

					if (zip_entry_noallocread(z, buffer.data(), buffer.size()) < 0)
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to read {0} from vfs.", file);
					}

					zip_entry_close(z);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to open zip entry for {0}.", file);
				}

				zip_close(z);

				return buffer;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find file '{0}' in vfs.", file);
			}

			return {};
		}

		template<meta::vfs_write_type FileType>
		inline bool VirtualFileSystem::write(FileType data, const std::string& file)
		{
			bool result = false;

			if (!data.empty() && !file.empty())
			{
				auto entry_opt = find(file);
				if (entry_opt.has_value())
				{
					// This should be copied, so its ok to reference after.
					ArchiveEntry ae = entry_opt.value();
					remove(file, ae);

					auto z = zip_open(ae.pack.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'a');
					if (zip_entry_open(z, ae.entry.c_str()) >= 0)
					{
						if (zip_entry_write(z, data.data(), data.size()) < 0)
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to write entry to zip for file '{0}'.", file);
						}
						else
						{
							result = true;
						}

						zip_entry_close(z);
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to open zip entry for {0}.", file);
					}
				}
				else
				{
					auto z = zip_open(m_datapack.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
					if (zip_entry_open(z, file.c_str()) == 0)
					{
						if (zip_entry_write(z, data.data(), data.size()) < 0)
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to write entry to zip for file '{0}'.", file);
						}
						else
						{
							result          = true;
							const auto path = std::filesystem::path(zip_entry_name(z));
							m_tree[path.filename().string()] =
								ArchiveEntry {.entry = zip_entry_name(z), .pack = m_datapack, .type = get_asset_type_from_path(path.string())};
						}

						zip_entry_close(z);
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to open zip entry for {0}.", file);
					}
					zip_close(z);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Passed empty data or filename to vfs to write.");
			}

			return result;
		}

		template<meta::vfs_read_type FileType>
		inline FileType VirtualFileSystem::read_disk(const std::string& file)
		{
			if (!file.empty())
			{
				std::filesystem::path path = file;
				if (!path.is_absolute())
				{
					path = GALAXY_ROOT_DIR / path;
				}

				std::ifstream ifs;

				if constexpr (std::is_same_v<FileType, meta::FSTextR>)
				{
					ifs.open(path.string(), std::ifstream::in);
				}
				else if constexpr (std::is_same_v<FileType, meta::FSBinaryR>)
				{
					ifs.open(path.string(), std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
				}

				if (ifs.good())
				{
					if constexpr (std::is_same_v<FileType, meta::FSTextR>)
					{
						std::stringstream buffer;
						buffer << ifs.rdbuf();
						ifs.close();

						return buffer.str();
					}
					else if constexpr (std::is_same_v<FileType, meta::FSBinaryR>)
					{
						meta::vector<char> buffer;

						const auto size = ifs.tellg();
						buffer.resize(size);

						ifs.seekg(0, std::ifstream::beg);
						ifs.read(&buffer[0], size);
						ifs.close();

						meta::FSBinaryR out;
						reinterpret_cast<meta::vector<char>&>(out) = std::move(buffer);

						return out;
					}
				}
				else
				{
					ifs.close();
					GALAXY_LOG(GALAXY_ERROR, "Failed to read file '{0}'.", file);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to read empty file from disk.");
			}

			return {};
		}

		template<meta::vfs_write_type FileType>
		inline bool VirtualFileSystem::write_disk(FileType data, const std::string& file)
		{
			if (!data.empty() && !file.empty())
			{
				std::filesystem::path path = file;
				if (!path.is_absolute())
				{
					path = GALAXY_ROOT_DIR / path;
				}

				std::ofstream ofs;

				if constexpr (std::is_same_v<FileType, meta::FSTextR>)
				{
					ofs.open(path.string(), std::ofstream::out | std::ofstream::trunc);
				}
				else if constexpr (std::is_same_v<FileType, meta::FSBinaryR>)
				{
					ofs.open(path.string(), std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
				}

				if (ofs.good())
				{
					if constexpr (std::is_same_v<FileType, meta::FSTextR>)
					{
						ofs << data;
					}
					else if constexpr (std::is_same_v<FileType, meta::FSBinaryR>)
					{
						ofs.write(data.data(), data.size());
					}

					ofs.close();
					return true;
				}
				else
				{
					ofs.close();
					GALAXY_LOG(GALAXY_ERROR, "Failed to save file data to disk '{0}'.", file);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to write incorrectly to disk.");
			}

			return false;
		}
	} // namespace fs
} // namespace galaxy

#endif
