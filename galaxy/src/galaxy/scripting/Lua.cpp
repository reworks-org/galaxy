///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

// https://github.com/skaarj1989/entt-meets-sol2/tree/main/examples

#include <sol/sol.hpp>

#include "galaxy/algorithm/Algorithms.hpp"
#include "galaxy/algorithm/Base64.hpp"
#include "galaxy/algorithm/GZip.hpp"
#include "galaxy/algorithm/Random.hpp"
#include "galaxy/algorithm/ZLib.hpp"

#include "galaxy/core/Config.hpp"
#include "galaxy/core/Guid.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "galaxy/error/Log.hpp"

#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/KeyRepeat.hpp"
#include "galaxy/events/KeyUp.hpp"
#include "galaxy/events/KeyChar.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"

#include "galaxy/fs/FileInfo.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "galaxy/graphics/Rect.hpp"

#include "galaxy/input/InputMods.hpp"
#include "galaxy/input/Keys.hpp"
#include "galaxy/input/Mouse.hpp"

#include "galaxy/platform/Subprocess.hpp"

#include "galaxy/state/SceneManager.hpp"

#include "galaxy/utils/Globals.hpp"
#include "galaxy/utils/StringUtils.hpp"

#include "Lua.hpp"

namespace galaxy
{
	void log_wrapper(error::LogLevel error_level, std::string_view message)
	{
		switch (error_level)
		{
			case error::LogLevel::INFO:
				GALAXY_LOG(GALAXY_INFO, "{0}", message);
				break;

			case error::LogLevel::DEBUG:
				GALAXY_LOG(GALAXY_DEBUG, "{0}", message);
				break;

			case error::LogLevel::WARNING:
				GALAXY_LOG(GALAXY_WARNING, "{0}", message);
				break;

			case error::LogLevel::ERROR_:
				GALAXY_LOG(GALAXY_ERROR, "{0}", message);
				break;

			case error::LogLevel::FATAL:
				GALAXY_LOG(GALAXY_FATAL, "{0}", message);
				break;
		}
	}

	namespace lua
	{
		void inject_galaxy_into_lua()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			/* ALGORITHM */
			lua.set_function("normalize", &algorithm::normalize<float>);
			lua.set_function("encode_base64", &algorithm::encode_base64);
			lua.set_function("decode_base64", &algorithm::decode_base64);
			lua.set_function("encode_gzip", &algorithm::encode_gzip);
			lua.set_function("decode_gzip", &algorithm::decode_gzip);
			lua.set_function("random_int", &algorithm::random<int>);
			lua.set_function("random_float", &algorithm::random<float>);
			lua.set_function("encode_zlib", &algorithm::encode_zlib);
			lua.set_function("decode_zlib", &algorithm::decode_zlib);

			/* CORE */
			auto config_type          = lua.new_usertype<core::Config>("Config", sol::no_constructor);
			config_type["load"]       = &core::Config::load;
			config_type["save"]       = &core::Config::save;
			config_type["get_bool"]   = sol::resolve<std::optional<bool>(const std::string&)>(&core::Config::get<bool>);
			config_type["get_int"]    = sol::resolve<std::optional<int>(const std::string&)>(&core::Config::get<int>);
			config_type["get_float"]  = sol::resolve<std::optional<float>(const std::string&)>(&core::Config::get<float>);
			config_type["get_string"] = sol::resolve<std::optional<std::string>(const std::string&)>(&core::Config::get<std::string>);
			config_type["get_section_bool"] =
				sol::resolve<std::optional<bool>(const std::string&, const std::string&, const std::string&)>(&core::Config::get<bool>);
			config_type["get_section_int"] =
				sol::resolve<std::optional<int>(const std::string&, const std::string&, const std::string&)>(&core::Config::get<int>);
			config_type["get_section_float"] =
				sol::resolve<std::optional<float>(const std::string&, const std::string&, const std::string&)>(&core::Config::get<float>);
			config_type["get_section_string"] =
				sol::resolve<std::optional<std::string>(const std::string&, const std::string&, const std::string&)>(&core::Config::get<std::string>);

			lua["service_config"] = std::ref(core::ServiceLocator<core::Config>::ref());

			auto guid_type         = lua.new_usertype<core::Guid>("Guid", sol::constructors<core::Guid()>());
			guid_type["as_string"] = &core::Guid::to_string;
			guid_type["is_empty"]  = &core::Guid::is_empty;

			/* ERROR */
			// clang-format off
			lua.new_enum<error::LogLevel>("LogLevels",
			{
				{"INFO", error::LogLevel::INFO},
				{"DEBUG", error::LogLevel::DEBUG},
				{"WARNING", error::LogLevel::WARNING},
				{"ERROR", error::LogLevel::ERROR_},
				{"FATAL", error::LogLevel::FATAL}
			});
			// clang-format on

			lua.set_function("galaxy_log", &log_wrapper);

			/* EVENTS */
			auto keychar_type      = lua.new_usertype<events::KeyChar>("KeyChar", sol::constructors<events::KeyChar()>());
			keychar_type["char"]   = &events::KeyChar::m_char;
			keychar_type["uichar"] = &events::KeyChar::m_uichar;

			auto keydown_type        = lua.new_usertype<events::KeyDown>("KeyDown", sol::constructors<events::KeyDown()>());
			keydown_type["keycode"]  = &events::KeyDown::m_keycode;
			keydown_type["mod"]      = &events::KeyDown::m_mod;
			keydown_type["scancode"] = &events::KeyDown::m_scancode;

			auto keyup_type        = lua.new_usertype<events::KeyUp>("KeyUp", sol::constructors<events::KeyUp()>());
			keyup_type["keycode"]  = &events::KeyUp::m_keycode;
			keyup_type["mod"]      = &events::KeyUp::m_mod;
			keyup_type["scancode"] = &events::KeyUp::m_scancode;

			auto keyrepeat_type        = lua.new_usertype<events::KeyRepeat>("KeyRepeat", sol::constructors<events::KeyRepeat()>());
			keyrepeat_type["keycode"]  = &events::KeyRepeat::m_keycode;
			keyrepeat_type["mod"]      = &events::KeyRepeat::m_mod;
			keyrepeat_type["scancode"] = &events::KeyRepeat::m_scancode;

			auto mousemoved_type    = lua.new_usertype<events::MouseMoved>("MouseMoved", sol::constructors<events::MouseMoved()>());
			mousemoved_type["xpos"] = &events::MouseMoved::m_xpos;
			mousemoved_type["ypos"] = &events::MouseMoved::m_ypos;

			auto mousepressed_type      = lua.new_usertype<events::MousePressed>("MousePressed", sol::constructors<events::MousePressed()>());
			mousepressed_type["button"] = &events::MousePressed::m_button;
			mousepressed_type["mod"]    = &events::MousePressed::m_mod;
			mousepressed_type["xpos"]   = &events::MousePressed::m_xpos;
			mousepressed_type["ypos"]   = &events::MousePressed::m_ypos;

			auto mousereleased_type      = lua.new_usertype<events::MouseReleased>("MouseReleased", sol::constructors<events::MouseReleased()>());
			mousereleased_type["button"] = &events::MouseReleased::m_button;
			mousereleased_type["mod"]    = &events::MouseReleased::m_mod;
			mousereleased_type["xpos"]   = &events::MouseReleased::m_xpos;
			mousereleased_type["ypos"]   = &events::MouseReleased::m_ypos;

			auto mousewheel_type    = lua.new_usertype<events::MouseWheel>("MouseWheel", sol::constructors<events::MouseWheel()>());
			mousewheel_type["xoff"] = &events::MouseWheel::m_xoff;
			mousewheel_type["yoff"] = &events::MouseWheel::m_yoff;

			auto windowresized_type      = lua.new_usertype<events::WindowResized>("WindowResized", sol::constructors<events::WindowResized()>());
			windowresized_type["width"]  = &events::WindowResized::m_width;
			windowresized_type["height"] = &events::WindowResized::m_height;

			/* FS */
			// clang-format off
			lua.new_enum<fs::FileInfo::Code>("FileInfoCodes",
			{
				{"FOUND", fs::FileInfo::Code::FOUND},
				{"NOT_FOUND", fs::FileInfo::Code::NOT_FOUND},
				{"NOT_IN_VFS", fs::FileInfo::Code::NOT_IN_VFS},
				{"NO_EXTENSION", fs::FileInfo::Code::NO_EXTENSION}
			});
			// clang-format on

			auto fileinfo_type      = lua.new_usertype<fs::FileInfo>("FileInfo", sol::constructors<fs::FileInfo()>());
			fileinfo_type["code"]   = &fs::FileInfo::m_code;
			fileinfo_type["string"] = &fs::FileInfo::m_string;
			// fileinfo_type["path"]   = &fs::FileInfo::m_path; // <-- Not Yet Supported.

			auto vfs_type             = lua.new_usertype<fs::VirtualFileSystem>("VirtualFileSystem", sol::no_constructor);
			vfs_type["find"]          = &fs::VirtualFileSystem::find;
			vfs_type["create_file"]   = &fs::VirtualFileSystem::create_file;
			vfs_type["create_folder"] = &fs::VirtualFileSystem::create_folder;
			vfs_type["open"]          = &fs::VirtualFileSystem::open;
			vfs_type["open_binary"]   = &fs::VirtualFileSystem::open_binary;
			vfs_type["save"]          = &fs::VirtualFileSystem::save;
			vfs_type["save_binary"]   = &fs::VirtualFileSystem::save_binary;
			vfs_type["remove"]        = &fs::VirtualFileSystem::remove;
			vfs_type["root"]          = &fs::VirtualFileSystem::root;

			lua["service_fs"] = std::ref(core::ServiceLocator<fs::VirtualFileSystem>::ref());

			/* GRAPHICS */
			auto frect_type = lua.new_usertype<graphics::fRect>("fRect", sol::constructors<graphics::fRect(), graphics::fRect(float, float, float, float)>());
			frect_type["x"] = &graphics::fRect::m_x;
			frect_type["y"] = &graphics::fRect::m_y;
			frect_type["width"]          = &graphics::fRect::m_width;
			frect_type["height"]         = &graphics::fRect::m_height;
			frect_type["contains_point"] = sol::resolve<bool(float, float)>(&graphics::fRect::contains);
			frect_type["contains_rect"]  = sol::resolve<bool(const graphics::fRect&)>(&graphics::fRect::contains);
			frect_type["overlaps"]       = &graphics::fRect::overlaps;

			auto irect_type      = lua.new_usertype<graphics::iRect>("iRect", sol::constructors<graphics::iRect(), graphics::iRect(int, int, int, int)>());
			irect_type["x"]      = &graphics::iRect::m_x;
			irect_type["y"]      = &graphics::iRect::m_y;
			irect_type["width"]  = &graphics::iRect::m_width;
			irect_type["height"] = &graphics::iRect::m_height;
			irect_type["contains_point"] = sol::resolve<bool(int, int)>(&graphics::iRect::contains);
			irect_type["contains_rect"]  = sol::resolve<bool(const graphics::iRect&)>(&graphics::iRect::contains);
			irect_type["overlaps"]       = &graphics::iRect::overlaps;

			/* INPUT */
			// clang-format off
			lua.new_enum<input::InputMods>("InputMods",
			{
				{"SHIFT", input::InputMods::SHIFT},
				{"CONTROL", input::InputMods::CONTROL},
				{"ALT", input::InputMods::ALT},
				{"SUPER", input::InputMods::SUPER},
				{"CAPS_LOCK", input::InputMods::CAPS_LOCK},
				{"NUM_LOCK", input::InputMods::NUM_LOCK}
			});
			// clang-format on

			// clang-format off
			lua.new_enum<input::Keys>("Keys",
			{
				{"UNKNOWN", input::Keys::UNKNOWN},
				{"SPACE", input::Keys::SPACE},
				{"APOSTROPHE", input::Keys::APOSTROPHE},
				{"COMMA", input::Keys::COMMA},
				{"MINUS", input::Keys::MINUS},
				{"PERIOD", input::Keys::PERIOD},
				{"SLASH", input::Keys::SLASH},
				{"_0", input::Keys::_0},
				{"_1", input::Keys::_1},
				{"_2", input::Keys::_2},
				{"_3", input::Keys::_3},
				{"_4", input::Keys::_4},
				{"_5", input::Keys::_5},
				{"_6", input::Keys::_6},
				{"_7", input::Keys::_7},
				{"_8", input::Keys::_8},
				{"_9", input::Keys::_9},
				{"SEMICOLON", input::Keys::SEMICOLON},
				{"EQUAL", input::Keys::EQUAL},
				{"A", input::Keys::A},
				{"B", input::Keys::B},
				{"C", input::Keys::C},
				{"D", input::Keys::D},
				{"E", input::Keys::E},
				{"F", input::Keys::F},
				{"G", input::Keys::G},
				{"H", input::Keys::H},
				{"I", input::Keys::I},
				{"J", input::Keys::J},
				{"K", input::Keys::K},
				{"L", input::Keys::L},
				{"M", input::Keys::M},
				{"N", input::Keys::N},
				{"O", input::Keys::O},
				{"P", input::Keys::P},
				{"Q", input::Keys::Q},
				{"R", input::Keys::R},
				{"S", input::Keys::S},
				{"T", input::Keys::T},
				{"U", input::Keys::U},
				{"V", input::Keys::V},
				{"W", input::Keys::W},
				{"X", input::Keys::X},
				{"Y", input::Keys::Y},
				{"Z", input::Keys::Z},
				{"LEFT_BRACKET", input::Keys::LEFT_BRACKET},
				{"BACKSLASH", input::Keys::BACKSLASH},
				{"RIGHT_BRACKET", input::Keys::RIGHT_BRACKET},
				{"GRAVE_ACCENT", input::Keys::GRAVE_ACCENT},
				{"WORLD_1", input::Keys::WORLD_1},
				{"WORLD_2", input::Keys::WORLD_2},
				{"ESCAPE", input::Keys::ESCAPE},
				{"ENTER", input::Keys::ENTER},
				{"TAB", input::Keys::TAB},
				{"BACKSPACE", input::Keys::BACKSPACE},
				{"INSERT", input::Keys::INSERT},
				{"DELETE", input::Keys::DELETE},
				{"RIGHT", input::Keys::RIGHT},
				{"LEFT", input::Keys::LEFT},
				{"DOWN", input::Keys::DOWN},
				{"UP", input::Keys::UP},
				{"PAGE_UP", input::Keys::PAGE_UP},
				{"PAGE_DOWN", input::Keys::PAGE_DOWN},
				{"HOME", input::Keys::HOME},
				{"END", input::Keys::END},
				{"CAPS_LOCK", input::Keys::CAPS_LOCK},
				{"SCROLL_LOCK", input::Keys::SCROLL_LOCK},
				{"NUM_LOCK", input::Keys::NUM_LOCK},
				{"PRINT_SCREEN", input::Keys::PRINT_SCREEN},
				{"PAUSE", input::Keys::PAUSE},
				{"F1", input::Keys::F1},
				{"F2", input::Keys::F2},
				{"F3", input::Keys::F3},
				{"F4", input::Keys::F4},
				{"F5", input::Keys::F5},
				{"F6", input::Keys::F6},
				{"F7", input::Keys::F7},
				{"F8", input::Keys::F8},
				{"F9", input::Keys::F9},
				{"F10", input::Keys::F10},
				{"F11", input::Keys::F11},
				{"F12", input::Keys::F12},
				{"F13", input::Keys::F13},
				{"F14", input::Keys::F14},
				{"F15", input::Keys::F15},
				{"F16", input::Keys::F16},
				{"F17", input::Keys::F17},
				{"F18", input::Keys::F18},
				{"F19", input::Keys::F19},
				{"F20", input::Keys::F20},
				{"F21", input::Keys::F21},
				{"F22", input::Keys::F22},
				{"F23", input::Keys::F23},
				{"F24", input::Keys::F24},
				{"F25", input::Keys::F25},
				{"KP_0", input::Keys::KP_0},
				{"KP_1", input::Keys::KP_1},
				{"KP_2", input::Keys::KP_2},
				{"KP_3", input::Keys::KP_3},
				{"KP_4", input::Keys::KP_4},
				{"KP_5", input::Keys::KP_5},
				{"KP_6", input::Keys::KP_6},
				{"KP_7", input::Keys::KP_7},
				{"KP_8", input::Keys::KP_8},
				{"KP_9", input::Keys::KP_9},
				{"KP_DECIMAL", input::Keys::KP_DECIMAL},
				{"KP_DIVIDE", input::Keys::KP_DIVIDE},
				{"KP_MULTIPLY", input::Keys::KP_MULTIPLY},
				{"KP_SUBTRACT", input::Keys::KP_SUBTRACT},
				{"KP_ADD", input::Keys::KP_ADD},
				{"KP_ENTER", input::Keys::KP_ENTER},
				{"KP_EQUAL", input::Keys::KP_EQUAL},
				{"LEFT_SHIFT", input::Keys::LEFT_SHIFT},
				{"LEFT_CONTROL", input::Keys::LEFT_CONTROL},
				{"LEFT_ALT", input::Keys::LEFT_ALT},
				{"LEFT_SUPER", input::Keys::LEFT_SUPER},
				{"RIGHT_SHIFT", input::Keys::RIGHT_SHIFT},
				{"RIGHT_CONTROL", input::Keys::RIGHT_CONTROL},
				{"RIGHT_ALT", input::Keys::RIGHT_ALT},
				{"RIGHT_SUPER", input::Keys::RIGHT_SUPER},
				{"MENU", input::Keys::MENU}
			});
			// clang-format on

			// clang-format off
			lua.new_enum<input::Mouse::Buttons>("MouseButtons",
			{
                {"UNKNOWN", input::Mouse::Buttons::UNKNOWN},
				{"BTN_1", input::Mouse::Buttons::BTN_1},
				{"BTN_2", input::Mouse::Buttons::BTN_2},
				{"BTN_3", input::Mouse::Buttons::BTN_3},
				{"BTN_4", input::Mouse::Buttons::BTN_4},
				{"BTN_5", input::Mouse::Buttons::BTN_5},
				{"BTN_6", input::Mouse::Buttons::BTN_6},
				{"BTN_7", input::Mouse::Buttons::BTN_7},
				{"BTN_8", input::Mouse::Buttons::BTN_8},
				{"BTN_LAST", input::Mouse::Buttons::BTN_LAST},
				{"BTN_LEFT", input::Mouse::Buttons::BTN_LEFT},
				{"BTN_RIGHT", input::Mouse::Buttons::BTN_RIGHT},
				{"BTN_MIDDLE", input::Mouse::Buttons::BTN_MIDDLE}
			});
			// clang-format on

			/* PLATFORM */
			auto subprocess_type         = lua.new_usertype<platform::Subprocess>("Subprocess",
                sol::constructors<platform::Subprocess(), platform::Subprocess(std::string_view, std::span<std::string> args)>());
			subprocess_type["alive"]     = &platform::Subprocess::alive;
			subprocess_type["create"]    = &platform::Subprocess::create;
			subprocess_type["destroy"]   = &platform::Subprocess::destroy;
			subprocess_type["join"]      = &platform::Subprocess::join;
			subprocess_type["terminate"] = &platform::Subprocess::terminate;

			/* STATE */
			auto scenemanager_type      = lua.new_usertype<state::SceneManager>("SceneManager", sol::no_constructor);
			scenemanager_type["change"] = &state::SceneManager::change;
			scenemanager_type["clear"]  = &state::SceneManager::clear;
			scenemanager_type["load"]   = &state::SceneManager::load;
			scenemanager_type["save"]   = &state::SceneManager::save;

			lua["galaxy_state_manager"] = std::ref(core::ServiceLocator<state::SceneManager>::ref());

			/* UTILS */
			lua.set("GALAXY_MAX_THREADS", GALAXY_MAX_THREADS);
			lua.set("GALAXY_DEFAULT_THREAD_COUNT", GALAXY_DEFAULT_THREAD_COUNT);
			lua.set("GALAXY_DT", GALAXY_DT);
			lua.set("GALAXY_ROOT_DIR", GALAXY_ROOT_DIR);

			lua.set_function("str_split", &strutils::split);
			lua.set_function("parse_codepoint", &strutils::parse_codepoint);
		}
	} // namespace lua
} // namespace galaxy