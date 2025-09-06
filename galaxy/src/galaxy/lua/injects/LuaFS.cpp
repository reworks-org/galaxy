///
/// LuaFS.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/fs/DialogButton.hpp"
#include "galaxy/fs/DialogIcons.hpp"
#include "galaxy/fs/DialogType.hpp"
#include "galaxy/fs/FileError.hpp"
#include "galaxy/fs/FileUtils.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

namespace galaxy
{
	namespace lua
	{
		std::string fileutils_read_wrapper(const std::string& filepath) noexcept
		{
			const auto result = fileutils::read(filepath);
			if (result)
			{
				return result.value();
			}

			const auto& error = result.error();
			error.log();

			return error.m_reason;
		}

		void inject_fs()
		{
			auto& lua = entt::locator<sol::state>::value();

			// clang-format off
			lua.new_enum<DialogButton>("DialogButton",
			{
				{"cancel_no", DialogButton::cancel_no},
				{"ok_yes", DialogButton::ok_yes},
				{"yes_no_cancel", DialogButton::yes_no_cancel}
			});

			lua.new_enum<DialogIcon>("DialogIcons",
			{
				{"info", DialogIcon::info},
				{"warning", DialogIcon::warning},
				{"error", DialogIcon::error}
			});

			lua.new_enum<DialogType>("DialogType",
			{
				{"ok", DialogType::ok},
				{"okcancel", DialogType::okcancel},
				{"yesno", DialogType::yesno},
				{"yesnocancel", DialogType::yesnocancel}
			});
			// clang-format on

			auto fileerror =
				lua.new_usertype<FileError>("FileError", sol::constructors<FileError(), FileError(const std::string&, const std::string&, const std::filesystem::path&)>());
			fileerror["log"]    = &FileError::log;
			fileerror["mode"]   = &FileError::m_mode;
			fileerror["reason"] = &FileError::m_reason;
			fileerror["path"]   = [](const FileError& self) {
                return self.m_path.string();
			};

			lua.set_function("fs_extension", &fileutils::extension);
			lua.set_function("fs_read", &fileutils_read_wrapper);
			lua.set_function("fs_write", &fileutils::write);
			lua.set_function("fs_open_url", &fileutils::open_url);

			auto vfs_type                    = lua.new_usertype<VirtualFileSystem>("VirtualFileSystem", sol::no_constructor);
			vfs_type["read"]                 = &VirtualFileSystem::read;
			vfs_type["read_binary"]          = &VirtualFileSystem::read_binary;
			vfs_type["write"]                = &VirtualFileSystem::write;
			vfs_type["write_binary"]         = &VirtualFileSystem::write_binary;
			vfs_type["write_raw"]            = &VirtualFileSystem::write_raw;
			vfs_type["mkdir"]                = &VirtualFileSystem::mkdir;
			vfs_type["remove"]               = &VirtualFileSystem::remove;
			vfs_type["exists"]               = &VirtualFileSystem::exists;
			vfs_type["is_dir"]               = &VirtualFileSystem::is_dir;
			vfs_type["list"]                 = &VirtualFileSystem::list;
			vfs_type["alert"]                = &VirtualFileSystem::alert;
			vfs_type["notification"]         = &VirtualFileSystem::notification;
			vfs_type["message_box"]          = &VirtualFileSystem::message_box;
			vfs_type["input_box"]            = &VirtualFileSystem::input_box;
			vfs_type["open_save_dialog"]     = &VirtualFileSystem::open_save_dialog;
			vfs_type["open_file_dialog"]     = &VirtualFileSystem::open_file_dialog;
			vfs_type["select_folder_dialog"] = &VirtualFileSystem::select_folder_dialog;
		}
	} // namespace lua
} // namespace galaxy
