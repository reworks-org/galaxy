///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/error/Log.hpp"

#include "LuaUtils.hpp"

enum class LuaLogLevels : int
{
	INFO,
	DEBUG,
	WARNING,
	ERROR,
	FATAL
};

void log_wrapper(LuaLogLevels error_level, std::string_view message)
{
	switch (error_level)
	{
		case LuaLogLevels::INFO:
			GALAXY_LOG(GALAXY_INFO, "{0}", message);
			break;

		case LuaLogLevels::DEBUG:
			GALAXY_LOG(GALAXY_DEBUG, "{0}", message);
			break;

		case LuaLogLevels::WARNING:
			GALAXY_LOG(GALAXY_WARNING, "{0}", message);
			break;

		case LuaLogLevels::ERROR:
			GALAXY_LOG(GALAXY_ERROR, "{0}", message);
			break;

		case LuaLogLevels::FATAL:
			GALAXY_LOG(GALAXY_FATAL, "{0}", message);
			break;

		default:
			GALAXY_LOG(GALAXY_INFO, "{0}", message);
			break;
	}
}

namespace galaxy
{
	namespace lua
	{
		void register_functions()
		{
			auto lua = SL_HANDLE.lua();

			// Error handling.
			// clang-format off
			lua->new_enum<LuaLogLevels>("gLogLevels",
			{
				{"INFO", LuaLogLevels::INFO},
				{"DEBUG", LuaLogLevels::DEBUG},
				{"WARNING", LuaLogLevels::WARNING},
				{"ERROR", LuaLogLevels::ERROR},
				{"FATAL", LuaLogLevels::FATAL}
			});
			// clang-format on

			lua->set_function("galaxy_log", &log_wrapper);
		}

		void register_audio()
		{
			auto lua = SL_HANDLE.lua();
		}

		void register_ecs()
		{
			auto lua = SL_HANDLE.lua();
		}

		void register_events()
		{
			auto lua = SL_HANDLE.lua();
		}

		void register_fs()
		{
			auto lua = SL_HANDLE.lua();
		}

		void register_graphics()
		{
			auto lua = SL_HANDLE.lua();
		}

		void register_mapping()
		{
			auto lua = SL_HANDLE.lua();
		}

		void register_math()
		{
			auto lua = SL_HANDLE.lua();
		}

		void register_physics()
		{
			auto lua = SL_HANDLE.lua();
		}

		void register_platform()
		{
			auto lua = SL_HANDLE.lua();
		}

		void register_res()
		{
			auto lua = SL_HANDLE.lua();
		}

		void register_scripting()
		{
			auto lua = SL_HANDLE.lua();
		}
	} // namespace lua
} // namespace galaxy