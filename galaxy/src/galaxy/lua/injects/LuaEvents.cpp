///
/// LuaEvents.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <entt_sol/dispatcher/bond.hpp>

#include "galaxy/events/GainedFocus.hpp"
#include "galaxy/events/KeyInput.hpp"
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

		auto gf_type        = lua.new_usertype<GainedFocus>(sol::constructors<GainedFocus()>());
		gf_type["type_id"]  = &entt::type_hash<GainedFocus>::value;
		gf_type["consume"]  = &GainedFocus::consume;
		gf_type["consumed"] = &GainedFocus::consumed;

		auto ki_type        = lua.new_usertype<KeyInput>("KeyInput", sol::constructors<KeyInput(std::string&&)>());
		ki_type["type_id"]  = &entt::type_hash<KeyInput>::value;
		ki_type["text"]     = &KeyInput::m_text;
		ki_type["consume"]  = &KeyInput::consume;
		ki_type["consumed"] = &KeyInput::consumed;

		auto kp_type = lua.new_usertype<KeyPressed>("KeyPressed", sol::constructors<KeyPressed(const SDL_Scancode, const Keys, const KeyMods, const unsigned short, const bool)>());
		kp_type["type_id"]  = &entt::type_hash<KeyPressed>::value;
		kp_type["key"]      = &KeyPressed::m_key;
		kp_type["mod"]      = &KeyPressed::m_mod;
		kp_type["raw"]      = &KeyPressed::m_raw;
		kp_type["repeat"]   = &KeyPressed::m_repeat;
		kp_type["scancode"] = &KeyPressed::m_scancode;
		kp_type["consume"]  = &KeyPressed::consume;
		kp_type["consumed"] = &KeyPressed::consumed;

		auto kr_type =
			lua.new_usertype<KeyReleased>("KeyReleased", sol::constructors<KeyReleased(const SDL_Scancode, const Keys, const KeyMods, const unsigned short, const bool)>());
		kr_type["type_id"]  = &entt::type_hash<KeyReleased>::value;
		kr_type["key"]      = &KeyReleased::m_key;
		kr_type["mod"]      = &KeyReleased::m_mod;
		kr_type["raw"]      = &KeyReleased::m_raw;
		kr_type["repeat"]   = &KeyReleased::m_repeat;
		kr_type["scancode"] = &KeyReleased::m_scancode;
		kr_type["consume"]  = &KeyReleased::consume;
		kr_type["consumed"] = &KeyReleased::consumed;

		auto lf_type        = lua.new_usertype<LostFocus>(sol::constructors<LostFocus()>());
		lf_type["type_id"]  = &entt::type_hash<LostFocus>::value;
		lf_type["consume"]  = &LostFocus::consume;
		lf_type["consumed"] = &LostFocus::consumed;

		auto mm_type        = lua.new_usertype<MouseMoved>("MouseMoved", sol::constructors<MouseMoved(const float, const float, const float, const float)>());
		mm_type["type_id"]  = &entt::type_hash<MouseMoved>::value;
		mm_type["consume"]  = &MouseMoved::consume;
		mm_type["consumed"] = &MouseMoved::consumed;

		auto mp_type        = lua.new_usertype<MousePressed>("MousePressed", sol::constructors<MousePressed(const float, const float, const int, const MouseButton)>());
		mp_type["type_id"]  = &entt::type_hash<MousePressed>::value;
		mp_type["clicks"]   = &MousePressed::m_clicks;
		mp_type["xpos"]     = &MousePressed::m_xpos;
		mp_type["ypos"]     = &MousePressed::m_ypos;
		mp_type["button"]   = &MousePressed::m_button;
		mp_type["consume"]  = &MousePressed::consume;
		mp_type["consumed"] = &MousePressed::consumed;

		auto mr_type        = lua.new_usertype<MouseReleased>("MouseReleased", sol::constructors<MouseReleased(const float, const float, const int, const MouseButton)>());
		mr_type["type_id"]  = &entt::type_hash<MouseReleased>::value;
		mr_type["clicks"]   = &MouseReleased::m_clicks;
		mr_type["xpos"]     = &MouseReleased::m_xpos;
		mr_type["ypos"]     = &MouseReleased::m_ypos;
		mr_type["button"]   = &MouseReleased::m_button;
		mr_type["consume"]  = &MouseReleased::consume;
		mr_type["consumed"] = &MouseReleased::consumed;

		auto mw_type = lua.new_usertype<MouseWheel>(
			"MouseWheel",
			sol::constructors<MouseWheel(const float, const float, const SDL_MouseWheelDirection, const float, const float, const int, const int)>()
		);
		mw_type["type_id"]  = &entt::type_hash<MouseWheel>::value;
		mw_type["amount_x"] = &MouseWheel::m_amount_x;
		mw_type["amount_y"] = &MouseWheel::m_amount_y;
		mw_type["mouse_x"]  = &MouseWheel::m_mouse_x;
		mw_type["mouse_y"]  = &MouseWheel::m_mouse_y;
		mw_type["total_x"]  = &MouseWheel::m_total_x;
		mw_type["total_y"]  = &MouseWheel::m_total_y;
		mw_type["consume"]  = &MouseWheel::consume;
		mw_type["consumed"] = &MouseWheel::consumed;

		auto wc_type        = lua.new_usertype<WindowClosed>("WindowClosed", sol::constructors<WindowClosed()>());
		wc_type["type_id"]  = &entt::type_hash<WindowClosed>::value;
		wc_type["consume"]  = &GainedFocus::consume;
		wc_type["consumed"] = &GainedFocus::consumed;

		auto wr_type        = lua.new_usertype<WindowResized>("WindowResized", sol::constructors<WindowResized(const int, const int)>());
		wr_type["type_id"]  = &entt::type_hash<WindowResized>::value;
		wr_type["height"]   = &WindowResized::m_height;
		wr_type["width"]    = &WindowResized::m_width;
		wr_type["consume"]  = &WindowResized::consume;
		wr_type["consumed"] = &WindowResized::consumed;

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
