///
/// LuaEvents.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <entt_sol/dispatcher.hpp>

#include "galaxy/events/GainedFocus.hpp"
#include "galaxy/events/KeyPressed.hpp"
#include "galaxy/events/KeyReleased.hpp"
#include "galaxy/events/LostFocus.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowClosed.hpp"
#include "galaxy/events/WindowResized.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_events() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		auto gf_type       = lua.new_usertype<GainedFocus>(sol::constructors<GainedFocus()>());
		gf_type["type_id"] = &entt::type_hash<GainedFocus>::value;

		auto kp_type        = lua.new_usertype<KeyPressed>("KeyPressed", sol::constructors<KeyPressed()>());
		kp_type["type_id"]  = &entt::type_hash<KeyPressed>::value;
		kp_type["handled"]  = &KeyPressed::m_handled;
		kp_type["key"]      = &KeyPressed::m_key;
		kp_type["mod"]      = &KeyPressed::m_mod;
		kp_type["raw"]      = &KeyPressed::m_raw;
		kp_type["repeat"]   = &KeyPressed::m_repeat;
		kp_type["scancode"] = &KeyPressed::m_scancode;

		auto kr_type        = lua.new_usertype<KeyReleased>("KeyReleased", sol::constructors<KeyReleased()>());
		kr_type["type_id"]  = &entt::type_hash<KeyReleased>::value;
		kr_type["handled"]  = &KeyReleased::m_handled;
		kr_type["key"]      = &KeyReleased::m_key;
		kr_type["mod"]      = &KeyReleased::m_mod;
		kr_type["raw"]      = &KeyReleased::m_raw;
		kr_type["repeat"]   = &KeyReleased::m_repeat;
		kr_type["scancode"] = &KeyReleased::m_scancode;

		auto lf_type       = lua.new_usertype<LostFocus>(sol::constructors<LostFocus()>());
		lf_type["type_id"] = &entt::type_hash<LostFocus>::value;

		auto mm_type       = lua.new_usertype<MouseMoved>("MouseMoved", sol::constructors<MouseMoved()>());
		mm_type["type_id"] = &entt::type_hash<MouseMoved>::value;

		auto mp_type       = lua.new_usertype<MousePressed>("MousePressed", sol::constructors<MousePressed()>());
		mp_type["type_id"] = &entt::type_hash<MousePressed>::value;
		mp_type["clicks"]  = &MousePressed::m_clicks;
		mp_type["handled"] = &MousePressed::m_handled;
		mp_type["xpos"]    = &MousePressed::m_xpos;
		mp_type["ypos"]    = &MousePressed::m_ypos;
		mp_type["button"]  = &MousePressed::m_button;

		auto mr_type       = lua.new_usertype<MouseReleased>("MouseReleased", sol::constructors<MouseReleased()>());
		mr_type["type_id"] = &entt::type_hash<MouseReleased>::value;
		mr_type["clicks"]  = &MouseReleased::m_clicks;
		mr_type["handled"] = &MouseReleased::m_handled;
		mr_type["xpos"]    = &MouseReleased::m_xpos;
		mr_type["ypos"]    = &MouseReleased::m_ypos;
		mr_type["button"]  = &MouseReleased::m_button;

		auto mw_type        = lua.new_usertype<MouseWheel>("MouseWheel", sol::constructors<MouseWheel()>());
		mw_type["type_id"]  = &entt::type_hash<MouseWheel>::value;
		mw_type["amount_x"] = &MouseWheel::m_amount_x;
		mw_type["amount_y"] = &MouseWheel::m_amount_y;
		mw_type["handled"]  = &MouseWheel::m_handled;
		mw_type["mouse_x"]  = &MouseWheel::m_mouse_x;
		mw_type["mouse_y"]  = &MouseWheel::m_mouse_y;
		mw_type["total_x"]  = &MouseWheel::m_total_x;
		mw_type["total_y"]  = &MouseWheel::m_total_y;

		auto wc_type       = lua.new_usertype<WindowClosed>("WindowClosed", sol::constructors<WindowClosed()>());
		wc_type["type_id"] = &entt::type_hash<WindowClosed>::value;

		auto wr_type       = lua.new_usertype<WindowResized>("WindowResized", sol::constructors<WindowResized()>());
		wr_type["type_id"] = &entt::type_hash<WindowResized>::value;
		wr_type["height"]  = &WindowResized::m_height;
		wr_type["width"]   = &WindowResized::m_width;

		entt_sol::register_meta_event<GainedFocus>();
		entt_sol::register_meta_event<KeyPressed>();
		entt_sol::register_meta_event<KeyReleased>();
		entt_sol::register_meta_event<LostFocus>();
		entt_sol::register_meta_event<MouseMoved>();
		entt_sol::register_meta_event<MousePressed>();
		entt_sol::register_meta_event<MouseReleased>();
		entt_sol::register_meta_event<MouseWheel>();
		entt_sol::register_meta_event<WindowClosed>();
		entt_sol::register_meta_event<WindowResized>();
	}
} // namespace galaxy
