///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/scripting/LuaEntity.hpp"

#include "LuaUtils.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Namespace that contains useful functions for interacting with Lua.
	///
	namespace Lua
	{
		void register_types()
		{
			// Service pointer.
			auto lua = SL_HANDLE.lua();

			// galaxy::LuaEntity
			lua->new_usertype<galaxy::LuaEntity>("LuaEntity", sol::constructors<LuaEntity(), LuaEntity(std::string_view), LuaEntity(sr::Entity)>(), "add_animation", &LuaEntity::add_animation, "add_music", &LuaEntity::add_music, "add_shader", &LuaEntity::add_shader, "add_sound", &LuaEntity::add_sound, "add_sprite", &LuaEntity::add_sprite, "add_spritebatch", &LuaEntity::add_spritebatch, "enable", &LuaEntity::enable, "disable", &LuaEntity::disable);
		}
	} // namespace Lua
} // namespace galaxy