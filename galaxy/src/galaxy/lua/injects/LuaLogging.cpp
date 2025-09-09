///
/// LuaError.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/logging/Log.hpp"
#include "galaxy/logging/PhysFSError.hpp"
#include "galaxy/logging/OpenGLError.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void log_wrapper(LogLevel error_level, std::string_view message)
	{
		switch (error_level)
		{
			case LogLevel::INFO:
				GALAXY_LOG(GALAXY_INFO, "{0}", message);
				break;

			case LogLevel::WARNING:
				GALAXY_LOG(GALAXY_WARN, "{0}", message);
				break;

			case LogLevel::ERROR:

				GALAXY_LOG(GALAXY_ERROR, "{0}", message);
				break;
		}
	}

	void Lua::inject_logging() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		// clang-format off
			// We dont have FATAL here because we dont want to throw exceptions inside lua.
			lua.new_enum<LogLevel>("LogLevels",
			{
				{"INFO", LogLevel::INFO},
				{"WARNING", LogLevel::WARNING},
				{"ERROR", LogLevel::ERROR},
			});
		// clang-format on

		lua.set_function("galaxy_log", &log_wrapper);
		lua.set_function("galaxy_log_physfs_check", sol::resolve<bool(const int)>(log::physfs_check));
		lua.set_function("galaxy_gl_errcode_as_string", &log::gl_errcode_as_string);
		lua.set_function("galaxy_gl_get_all_errors", &log::gl_get_all_errors);
		lua.set_function("galaxy_gl_add_error", &log::gl_add_error);
	}
} // namespace galaxy
