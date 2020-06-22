///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <sol/sol.hpp>

#include "galaxy/scripting/Command.hpp"
#include "galaxy/scripting/LuaEntity.hpp"
#include "galaxy/core/ServiceLocator.hpp"

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
		void registerTypes() noexcept
		{
			// Service pointer.
			auto lua = SL_HANDLE.lua();

			// galaxy::command
			lua->new_usertype<galaxy::Command>("Command",
				"exec", &Command::exec,
				"undo", &Command::undo
			);

			// galaxy::LuaEntity
			lua->new_usertype<galaxy::LuaEntity>("LuaEntity", sol::constructors<LuaEntity(), LuaEntity(const std::string&), LuaEntity(sr::Entity)>(),
				"addSpriteComponent", &LuaEntity::addSpriteComponent,
				"addTransformComponent", &LuaEntity::addTransformComponent,
				"addShaderComponent", &LuaEntity::addShaderComponent,
				"enable", &LuaEntity::enable,
				"disable", &LuaEntity::disable
			);
		}
	}
}