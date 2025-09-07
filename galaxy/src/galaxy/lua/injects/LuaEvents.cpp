///
/// LuaEvents.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt_sol/dispatcher.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/events/ContentScale.hpp"
#include "galaxy/events/KeyChar.hpp"
#include "galaxy/events/KeyPress.hpp"
#include "galaxy/events/KeyRepeat.hpp"
#include "galaxy/events/MouseEnter.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowClosed.hpp"
#include "galaxy/events/WindowResized.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_events()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			auto contentscale_type       = lua.new_usertype<events::ContentScale>("ContentScale", sol::constructors<events::ContentScale()>());
			contentscale_type["type_id"] = &entt::type_hash<events::ContentScale>::value;
			contentscale_type["xscale"]  = &events::ContentScale::xscale;
			contentscale_type["yscale"]  = &events::ContentScale::yscale;

			auto keychar_type         = lua.new_usertype<events::KeyChar>("KeyChar", sol::constructors<events::KeyChar()>());
			keychar_type["type_id"]   = &entt::type_hash<events::KeyChar>::value;
			keychar_type["codepoint"] = &events::KeyChar::codepoint;
			keychar_type["handled"]   = &events::KeyChar::handled;

			auto keypress_type        = lua.new_usertype<events::KeyPress>("KeyPress", sol::constructors<events::KeyPress()>());
			keypress_type["type_id"]  = &entt::type_hash<events::KeyPress>::value;
			keypress_type["keycode"]  = &events::KeyPress::keycode;
			keypress_type["mod"]      = &events::KeyPress::mod;
			keypress_type["scancode"] = &events::KeyPress::scancode;
			keypress_type["handled"]  = &events::KeyPress::handled;
			keypress_type["pressed"]  = &events::KeyPress::pressed;
			keypress_type["repeat"]   = &events::KeyPress::repeat;

			auto keyrepeat_type        = lua.new_usertype<events::KeyRepeat>("KeyRepeat", sol::constructors<events::KeyRepeat()>());
			keyrepeat_type["type_id"]  = &entt::type_hash<events::KeyRepeat>::value;
			keyrepeat_type["keycode"]  = &events::KeyRepeat::keycode;
			keyrepeat_type["mod"]      = &events::KeyRepeat::mod;
			keyrepeat_type["scancode"] = &events::KeyRepeat::scancode;
			keyrepeat_type["handled"]  = &events::KeyRepeat::handled;

			auto mouseentered_type       = lua.new_usertype<events::MouseEnter>("MouseEnter", sol::constructors<events::MouseEnter()>());
			mouseentered_type["type_id"] = &entt::type_hash<events::MouseEnter>::value;
			mouseentered_type["entered"] = &events::MouseEnter::entered;
			mouseentered_type["handled"] = &events::MouseEnter::handled;

			auto mousemoved_type       = lua.new_usertype<events::MouseMoved>("MouseMoved", sol::constructors<events::MouseMoved()>());
			mousemoved_type["type_id"] = &entt::type_hash<events::MouseMoved>::value;
			mousemoved_type["xpos"]    = &events::MouseMoved::xpos;
			mousemoved_type["ypos"]    = &events::MouseMoved::ypos;
			mousemoved_type["handled"] = &events::MouseMoved::handled;

			auto mousepressed_type       = lua.new_usertype<events::MousePressed>("MousePressed", sol::constructors<events::MousePressed()>());
			mousepressed_type["type_id"] = &entt::type_hash<events::MousePressed>::value;
			mousepressed_type["button"]  = &events::MousePressed::button;
			mousepressed_type["mod"]     = &events::MousePressed::mod;
			mousepressed_type["xpos"]    = &events::MousePressed::xpos;
			mousepressed_type["ypos"]    = &events::MousePressed::ypos;
			mousepressed_type["handled"] = &events::MousePressed::handled;

			auto mousereleased_type       = lua.new_usertype<events::MouseReleased>("MouseReleased", sol::constructors<events::MouseReleased()>());
			mousereleased_type["type_id"] = &entt::type_hash<events::MouseReleased>::value;
			mousereleased_type["button"]  = &events::MouseReleased::button;
			mousereleased_type["mod"]     = &events::MouseReleased::mod;
			mousereleased_type["xpos"]    = &events::MouseReleased::xpos;
			mousereleased_type["ypos"]    = &events::MouseReleased::ypos;
			mousereleased_type["handled"] = &events::MouseReleased::handled;

			auto mousewheel_type       = lua.new_usertype<events::MouseWheel>("MouseWheel", sol::constructors<events::MouseWheel()>());
			mousewheel_type["type_id"] = &entt::type_hash<events::MouseWheel>::value;
			mousewheel_type["xoff"]    = &events::MouseWheel::xoff;
			mousewheel_type["yoff"]    = &events::MouseWheel::yoff;
			mousewheel_type["handled"] = &events::MouseWheel::handled;

			auto windowclosed_type       = lua.new_usertype<events::WindowClosed>("WindowClosed", sol::constructors<events::WindowClosed()>());
			windowclosed_type["type_id"] = &entt::type_hash<events::WindowClosed>::value;

			auto windowresized_type       = lua.new_usertype<events::WindowResized>("WindowResized", sol::constructors<events::WindowResized()>());
			windowresized_type["type_id"] = &entt::type_hash<events::WindowResized>::value;
			windowresized_type["width"]   = &events::WindowResized::width;
			windowresized_type["height"]  = &events::WindowResized::height;

			entt_sol::register_meta_event<events::ContentScale>();
			entt_sol::register_meta_event<events::KeyChar>();
			entt_sol::register_meta_event<events::KeyPress>();
			entt_sol::register_meta_event<events::KeyRepeat>();
			entt_sol::register_meta_event<events::MouseEnter>();
			entt_sol::register_meta_event<events::MouseMoved>();
			entt_sol::register_meta_event<events::MousePressed>();
			entt_sol::register_meta_event<events::MouseReleased>();
			entt_sol::register_meta_event<events::MouseWheel>();
			entt_sol::register_meta_event<events::WindowClosed>();
			entt_sol::register_meta_event<events::WindowResized>();
		}
	} // namespace lua
} // namespace galaxy
