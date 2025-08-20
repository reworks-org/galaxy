///
/// LuaFS.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/fs/FileError.hpp"
#include "galaxy/fs/FileUtils.hpp"

// #include "galaxy/fs/VirtualFileSystem.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_fs()
		{
			auto& lua = entt::locator<sol::state>::value();

			auto fileerror =
				lua.new_usertype<FileError>("FileError", sol::constructors<FileError(), FileError(const std::string&, const std::string&, const std::filesystem::path&)>());
			fileerror["log"]    = &FileError::log;
			fileerror["mode"]   = &FileError::m_mode;
			fileerror["reason"] = &FileError::m_reason;
			fileerror["path"]   = [](const FileError& self) {
                return self.m_path.string();
			};

			/*lua.set_function("fs_extension", &fileutils::extension);
			lua.set_function("fs_read", &fileutils::read);
			lua.set_function("fs_write", &fileutils::write);*/

			//// clang-format off
			// lua.new_enum<fs::DialogButton>("DialogButton",
			//{
			//	{"cancel_no", fs::DialogButton::cancel_no},
			//	{"ok_yes", fs::DialogButton::ok_yes},
			//	{"yes_no_cancel", fs::DialogButton::yes_no_cancel}
			// });

			// lua.new_enum<fs::DialogIcon>("DialogIcons",
			//{
			//	{"info", fs::DialogIcon::info},
			//	{"warning", fs::DialogIcon::warning},
			//	{"error", fs::DialogIcon::error}
			// });

			// lua.new_enum<fs::DialogType>("DialogType",
			//{
			//	{"ok", fs::DialogType::ok},
			//	{"okcancel", fs::DialogType::okcancel},
			//	{"yesno", fs::DialogType::yesno},
			//	{"yesnocancel", fs::DialogType::yesnocancel}
			// });
			//// clang-format on

			// auto physfs_stream       = lua.new_usertype<fs::PhysfsStream>("PhysfsStream", sol::constructors<fs::PhysfsStream(), fs::PhysfsStream(const std::string&)>());
			// physfs_stream["close"]   = &fs::PhysfsStream::close;
			// physfs_stream["getSize"] = &fs::PhysfsStream::getSize;
			// physfs_stream["is_open"] = &fs::PhysfsStream::is_open;
			// physfs_stream["open"]    = &fs::PhysfsStream::open;
			// physfs_stream["read"]    = &fs::PhysfsStream::read;
			// physfs_stream["seek"]    = &fs::PhysfsStream::seek;
			// physfs_stream["tell"]    = &fs::PhysfsStream::tell;

			// auto vfs_type                    = lua.new_usertype<fs::VirtualFileSystem>("VirtualFileSystem", sol::no_constructor);
			// vfs_type["read"]                 = &fs::VirtualFileSystem::read;
			// vfs_type["read_binary"]          = &fs::VirtualFileSystem::read_binary;
			// vfs_type["write"]                = &fs::VirtualFileSystem::write;
			// vfs_type["write_binary"]         = &fs::VirtualFileSystem::write_binary;
			// vfs_type["write_raw"]            = &fs::VirtualFileSystem::write_raw;
			// vfs_type["mkdir"]                = &fs::VirtualFileSystem::mkdir;
			// vfs_type["remove"]               = &fs::VirtualFileSystem::remove;
			// vfs_type["exists"]               = &fs::VirtualFileSystem::exists;
			// vfs_type["is_dir"]               = &fs::VirtualFileSystem::is_dir;
			// vfs_type["list"]                 = &fs::VirtualFileSystem::list;
			// vfs_type["alert"]                = &fs::VirtualFileSystem::alert;
			// vfs_type["notification"]         = &fs::VirtualFileSystem::notification;
			// vfs_type["message_box"]          = &fs::VirtualFileSystem::message_box;
			// vfs_type["input_box"]            = &fs::VirtualFileSystem::input_box;
			// vfs_type["open_save_dialog"]     = &fs::VirtualFileSystem::open_save_dialog;
			// vfs_type["open_file_dialog"]     = &fs::VirtualFileSystem::open_file_dialog;
			// vfs_type["select_folder_dialog"] = &fs::VirtualFileSystem::select_folder_dialog;
			// vfs_type["get_file_extension"]   = &fs::VirtualFileSystem::get_file_extension;
		}
	} // namespace lua
} // namespace galaxy
