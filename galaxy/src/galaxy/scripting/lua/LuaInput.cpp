///
/// LuaInput.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/input/CameraController.hpp"
#include "galaxy/input/Clipboard.hpp"
#include "galaxy/input/Cursor.hpp"
#include "galaxy/input/Input.hpp"
#include "galaxy/input/InputMods.hpp"
#include "galaxy/input/Keyboard.hpp"
#include "galaxy/input/Mouse.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_input()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			auto camera_controller      = lua.new_usertype<input::CameraController>("CameraController", sol::constructors<input::CameraController(graphics::Camera&)>());
			camera_controller["camera"] = &input::CameraController::m_camera;

			auto clipboard_type   = lua.new_usertype<input::Clipboard>("Clipboard", sol::no_constructor);
			clipboard_type["set"] = &input::Clipboard::set;
			clipboard_type["get"] = &input::Clipboard::get;

			auto cursor_type                       = lua.new_usertype<input::Cursor>("Cursor", sol::no_constructor);
			cursor_type["toggle"]                  = &input::Cursor::toggle;
			cursor_type["load_custom"]             = &input::Cursor::load_custom;
			cursor_type["load_custom_mem"]         = &input::Cursor::load_custom_mem;
			cursor_type["use_custom"]              = &input::Cursor::use_custom;
			cursor_type["use_pointer"]             = &input::Cursor::use_pointer;
			cursor_type["use_hand"]                = &input::Cursor::use_hand;
			cursor_type["use_cross"]               = &input::Cursor::use_cross;
			cursor_type["use_text"]                = &input::Cursor::use_text;
			cursor_type["use_custom_else_pointer"] = &input::Cursor::use_custom_else_pointer;
			cursor_type["within_window"]           = &input::Cursor::within_window;
			cursor_type["destroy"]                 = &input::Cursor::destroy;

			lua.set_function("galaxy_input_key_down", &input::Input::key_down);
			lua.set_function("galaxy_input_mouse_down", &input::Input::mouse_button_down);
			lua.set_function("galaxy_input_cursor_pos", &input::Input::get_cursor_pos);

			lua.set("galaxy_camera_forward", &input::CameraKeys::FORWARD);
			lua.set("galaxy_camera_backward", &input::CameraKeys::BACKWARD);
			lua.set("galaxy_camera_left", &input::CameraKeys::LEFT);
			lua.set("galaxy_camera_right", &input::CameraKeys::RIGHT);
			lua.set("galaxy_camera_rotate_left", &input::CameraKeys::ROTATE_LEFT);
			lua.set("galaxy_camera_rotate_right", &input::CameraKeys::ROTATE_RIGHT);

			// clang-format off
			lua.new_enum<input::InputMods>("InputMods",
			{
				{"UNKNOWN", input::InputMods::UNKNOWN},
				{"SHIFT", input::InputMods::SHIFT},
				{"CONTROL", input::InputMods::CONTROL},
				{"ALT", input::InputMods::ALT},
				{"SUPER", input::InputMods::SUPER},
				{"CAPS_LOCK", input::InputMods::CAPS_LOCK},
				{"NUM_LOCK", input::InputMods::NUM_LOCK}
			});
			// clang-format on

			lua.set_function("galaxy_int_to_mod", &input::int_to_mod);
			lua.set_function("galaxy_mod_to_int", &input::mod_to_int);

			auto keyboard_type                     = lua.new_usertype<input::Keyboard>("Keyboard", sol::no_constructor);
			keyboard_type["begin_text_input"]      = &input::Keyboard::begin_text_input;
			keyboard_type["end_text_input"]        = &input::Keyboard::end_text_input;
			keyboard_type["enable_sticky_keys"]    = &input::Keyboard::enable_sticky_keys;
			keyboard_type["disable_sticky_keys"]   = &input::Keyboard::disable_sticky_keys;
			keyboard_type["is_text_input_enabled"] = &input::Keyboard::is_text_input_enabled;
			keyboard_type["get_scancode"]          = &input::Keyboard::get_scancode;
			keyboard_type["get_key_name"]          = &input::Keyboard::get_key_name;
			keyboard_type["get_scancode_name"]     = &input::Keyboard::get_scancode_name;

			// clang-format off
			lua.new_enum<input::Keys>("Keys",
			{
				{"KEY_UNKNOWN", input::Keys::KEY_UNKNOWN},
				{"KEY_SPACE", input::Keys::KEY_SPACE},
				{"KEY_APOSTROPHE", input::Keys::KEY_APOSTROPHE},
				{"KEY_COMMA", input::Keys::KEY_COMMA},
				{"KEY_MINUS", input::Keys::KEY_MINUS},
				{"KEY_PERIOD", input::Keys::KEY_PERIOD},
				{"KEY_SLASH", input::Keys::KEY_SLASH},
				{"KEY_0", input::Keys::KEY_0},
				{"KEY_1", input::Keys::KEY_1},
				{"KEY_2", input::Keys::KEY_2},
				{"KEY_3", input::Keys::KEY_3},
				{"KEY_4", input::Keys::KEY_4},
				{"KEY_5", input::Keys::KEY_5},
				{"KEY_6", input::Keys::KEY_6},
				{"KEY_7", input::Keys::KEY_7},
				{"KEY_8", input::Keys::KEY_8},
				{"KEY_9", input::Keys::KEY_9},
				{"KEY_SEMICOLON", input::Keys::KEY_SEMICOLON},
				{"KEY_EQUAL", input::Keys::KEY_EQUAL},
				{"KEY_A", input::Keys::KEY_A},
				{"KEY_B", input::Keys::KEY_B},
				{"KEY_C", input::Keys::KEY_C},
				{"KEY_D", input::Keys::KEY_D},
				{"KEY_E", input::Keys::KEY_E},
				{"KEY_F", input::Keys::KEY_F},
				{"KEY_G", input::Keys::KEY_G},
				{"KEY_H", input::Keys::KEY_H},
				{"KEY_I", input::Keys::KEY_I},
				{"KEY_J", input::Keys::KEY_J},
				{"KEY_K", input::Keys::KEY_K},
				{"KEY_L", input::Keys::KEY_L},
				{"KEY_M", input::Keys::KEY_M},
				{"KEY_N", input::Keys::KEY_N},
				{"KEY_O", input::Keys::KEY_O},
				{"KEY_P", input::Keys::KEY_P},
				{"KEY_Q", input::Keys::KEY_Q},
				{"KEY_R", input::Keys::KEY_R},
				{"KEY_S", input::Keys::KEY_S},
				{"KEY_T", input::Keys::KEY_T},
				{"KEY_U", input::Keys::KEY_U},
				{"KEY_V", input::Keys::KEY_V},
				{"KEY_W", input::Keys::KEY_W},
				{"KEY_X", input::Keys::KEY_X},
				{"KEY_Y", input::Keys::KEY_Y},
				{"KEY_Z", input::Keys::KEY_Z},
				{"KEY_LEFT_BRACKET", input::Keys::KEY_LEFT_BRACKET},
				{"KEY_BACKSLASH", input::Keys::KEY_BACKSLASH},
				{"KEY_RIGHT_BRACKET", input::Keys::KEY_RIGHT_BRACKET},
				{"KEY_GRAVE_ACCENT", input::Keys::KEY_GRAVE_ACCENT},
				{"KEY_WORLD_1", input::Keys::KEY_WORLD_1},
				{"KEY_WORLD_2", input::Keys::KEY_WORLD_2},
				{"KEY_ESCAPE", input::Keys::KEY_ESCAPE},
				{"KEY_ENTER", input::Keys::KEY_ENTER},
				{"KEY_TAB", input::Keys::KEY_TAB},
				{"KEY_BACKSPACE", input::Keys::KEY_BACKSPACE},
				{"KEY_INSERT", input::Keys::KEY_INSERT},
				{"KEY_DELETE", input::Keys::KEY_DELETE},
				{"KEY_RIGHT", input::Keys::KEY_RIGHT},
				{"KEY_LEFT", input::Keys::KEY_LEFT},
				{"KEY_DOWN", input::Keys::KEY_DOWN},
				{"KEY_UP", input::Keys::KEY_UP},
				{"KEY_PAGE_UP", input::Keys::KEY_PAGE_UP},
				{"KEY_PAGE_DOWN", input::Keys::KEY_PAGE_DOWN},
				{"KEY_HOME", input::Keys::KEY_HOME},
				{"KEY_END", input::Keys::KEY_END},
				{"KEY_CAPS_LOCK", input::Keys::KEY_CAPS_LOCK},
				{"KEY_SCROLL_LOCK", input::Keys::KEY_SCROLL_LOCK},
				{"KEY_NUM_LOCK", input::Keys::KEY_NUM_LOCK},
				{"KEY_PRINT_SCREEN", input::Keys::KEY_PRINT_SCREEN},
				{"KEY_PAUSE", input::Keys::KEY_PAUSE},
				{"KEY_F1", input::Keys::KEY_F1},
				{"KEY_F2", input::Keys::KEY_F2},
				{"KEY_F3", input::Keys::KEY_F3},
				{"KEY_F4", input::Keys::KEY_F4},
				{"KEY_F5", input::Keys::KEY_F5},
				{"KEY_F6", input::Keys::KEY_F6},
				{"KEY_F7", input::Keys::KEY_F7},
				{"KEY_F8", input::Keys::KEY_F8},
				{"KEY_F9", input::Keys::KEY_F9},
				{"KEY_F10", input::Keys::KEY_F10},
				{"KEY_F11", input::Keys::KEY_F11},
				{"KEY_F12", input::Keys::KEY_F12},
				{"KEY_F13", input::Keys::KEY_F13},
				{"KEY_F14", input::Keys::KEY_F14},
				{"KEY_F15", input::Keys::KEY_F15},
				{"KEY_F16", input::Keys::KEY_F16},
				{"KEY_F17", input::Keys::KEY_F17},
				{"KEY_F18", input::Keys::KEY_F18},
				{"KEY_F19", input::Keys::KEY_F19},
				{"KEY_F20", input::Keys::KEY_F20},
				{"KEY_F21", input::Keys::KEY_F21},
				{"KEY_F22", input::Keys::KEY_F22},
				{"KEY_F23", input::Keys::KEY_F23},
				{"KEY_F24", input::Keys::KEY_F24},
				{"KEY_F25", input::Keys::KEY_F25},
				{"KEY_KP_0", input::Keys::KEY_KP_0},
				{"KEY_KP_1", input::Keys::KEY_KP_1},
				{"KEY_KP_2", input::Keys::KEY_KP_2},
				{"KEY_KP_3", input::Keys::KEY_KP_3},
				{"KEY_KP_4", input::Keys::KEY_KP_4},
				{"KEY_KP_5", input::Keys::KEY_KP_5},
				{"KEY_KP_6", input::Keys::KEY_KP_6},
				{"KEY_KP_7", input::Keys::KEY_KP_7},
				{"KEY_KP_8", input::Keys::KEY_KP_8},
				{"KEY_KP_9", input::Keys::KEY_KP_9},
				{"KEY_KP_DECIMAL", input::Keys::KEY_KP_DECIMAL},
				{"KEY_KP_DIVIDE", input::Keys::KEY_KP_DIVIDE},
				{"KEY_KP_MULTIPLY", input::Keys::KEY_KP_MULTIPLY},
				{"KEY_KP_SUBTRACT", input::Keys::KEY_KP_SUBTRACT},
				{"KEY_KP_ADD", input::Keys::KEY_KP_ADD},
				{"KEY_KP_ENTER", input::Keys::KEY_KP_ENTER},
				{"KEY_KP_EQUAL", input::Keys::KEY_KP_EQUAL},
				{"KEY_LEFT_SHIFT", input::Keys::KEY_LEFT_SHIFT},
				{"KEY_LEFT_CONTROL", input::Keys::KEY_LEFT_CONTROL},
				{"KEY_LEFT_ALT", input::Keys::KEY_LEFT_ALT},
				{"KEY_LEFT_SUPER", input::Keys::KEY_LEFT_SUPER},
				{"KEY_RIGHT_SHIFT", input::Keys::KEY_RIGHT_SHIFT},
				{"KEY_RIGHT_CONTROL", input::Keys::KEY_RIGHT_CONTROL},
				{"KEY_RIGHT_ALT", input::Keys::KEY_RIGHT_ALT},
				{"KEY_RIGHT_SUPER", input::Keys::KEY_RIGHT_SUPER},
				{"KEY_MENU", input::Keys::KEY_MENU}
			});
			// clang-format on

			lua.set_function("galaxy_int_to_key", &input::int_to_key);
			lua.set_function("galaxy_key_to_int", &input::key_to_int);

			auto mouse_type                    = lua.new_usertype<input::Mouse>("Mouse", sol::no_constructor);
			mouse_type["disable_sticky_mouse"] = &input::Mouse::disable_sticky_mouse;
			mouse_type["enable_sticky_mouse"]  = &input::Mouse::enable_sticky_mouse;
			mouse_type["get_pos"]              = &input::Mouse::get_pos;

			// clang-format off
			lua.new_enum<input::MouseButtons>("MouseButtons",
			{
				{"UNKNOWN", input::MouseButtons::UNKNOWN},
				{"BTN_1", input::MouseButtons::BTN_1},
				{"BTN_2", input::MouseButtons::BTN_2},
				{"BTN_3", input::MouseButtons::BTN_3},
				{"BTN_4", input::MouseButtons::BTN_4},
				{"BTN_5", input::MouseButtons::BTN_5},
				{"BTN_6", input::MouseButtons::BTN_6},
				{"BTN_7", input::MouseButtons::BTN_7},
				{"BTN_8", input::MouseButtons::BTN_8},
				{"BTN_LAST", input::MouseButtons::BTN_LAST},
				{"BTN_LEFT", input::MouseButtons::BTN_LEFT},
				{"BTN_RIGHT", input::MouseButtons::BTN_RIGHT},
				{"BTN_MIDDLE", input::MouseButtons::BTN_MIDDLE}
			});
			// clang-format on

			lua.set_function("galaxy_int_to_mouse", &input::int_to_mouse);
			lua.set_function("galaxy_mouse_to_int", &input::mouse_to_int);
		}
	} // namespace lua
} // namespace galaxy
