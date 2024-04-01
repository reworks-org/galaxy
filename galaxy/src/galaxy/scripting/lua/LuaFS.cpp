///
/// LuaFS.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_fs()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			// clang-format off
			lua.new_enum<fs::DialogButton>("DialogButton",
			{
				{"cancel_no", fs::DialogButton::cancel_no},
				{"ok_yes", fs::DialogButton::ok_yes},
				{"yes_no_cancel", fs::DialogButton::yes_no_cancel}
			});

			lua.new_enum<fs::DialogIcon>("DialogIcons",
			{
				{"info", fs::DialogIcon::info},
				{"warning", fs::DialogIcon::warning},
				{"error", fs::DialogIcon::error}
			});

			lua.new_enum<fs::DialogType>("DialogType",
			{
				{"ok", fs::DialogType::ok},
				{"okcancel", fs::DialogType::okcancel},
				{"yesno", fs::DialogType::yesno},
				{"yesnocancel", fs::DialogType::yesnocancel}
			});
			// clang-format on

			auto vfs_type                    = lua.new_usertype<fs::VirtualFileSystem>("VirtualFileSystem", sol::no_constructor);
			vfs_type["alert"]                = &fs::VirtualFileSystem::alert;
			vfs_type["exists"]               = &fs::VirtualFileSystem::exists;
			vfs_type["input_box"]            = &fs::VirtualFileSystem::input_box;
			vfs_type["is_dir"]               = &fs::VirtualFileSystem::is_dir;
			vfs_type["list"]                 = &fs::VirtualFileSystem::list;
			vfs_type["message_box"]          = &fs::VirtualFileSystem::message_box;
			vfs_type["mkdir"]                = &fs::VirtualFileSystem::mkdir;
			vfs_type["notification"]         = &fs::VirtualFileSystem::notification;
			vfs_type["open_file_dialog"]     = &fs::VirtualFileSystem::open_file_dialog;
			vfs_type["open_save_dialog"]     = &fs::VirtualFileSystem::open_save_dialog;
			vfs_type["read"]                 = &fs::VirtualFileSystem::read;
			vfs_type["read_binary"]          = &fs::VirtualFileSystem::read_binary;
			vfs_type["remove"]               = &fs::VirtualFileSystem::remove;
			vfs_type["select_folder_dialog"] = &fs::VirtualFileSystem::select_folder_dialog;
			vfs_type["write"]                = &fs::VirtualFileSystem::write;
			vfs_type["write_binary"]         = &fs::VirtualFileSystem::write_binary;
			vfs_type["write_raw"]            = &fs::VirtualFileSystem::write_raw;
		}
	} // namespace lua
} // namespace galaxy
