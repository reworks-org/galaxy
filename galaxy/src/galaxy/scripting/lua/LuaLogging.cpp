///
/// LuaError.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/OpenGLError.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
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

				case error::LogLevel::ERROR:
					GALAXY_LOG(GALAXY_ERROR, "{0}", message);
					break;

				case error::LogLevel::FATAL:
					GALAXY_LOG(GALAXY_FATAL, "{0}", message);
					break;
			}
		}

		void inject_error()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			// clang-format off
			lua.new_enum<error::LogLevel>("LogLevels",
			{
				{"INFO", error::LogLevel::INFO},
				{"DEBUG", error::LogLevel::DEBUG},
				{"WARNING", error::LogLevel::WARNING},
				{"ERROR", error::LogLevel::ERROR},
				{"FATAL", error::LogLevel::FATAL}
			});
			// clang-format on

			lua.set_function("galaxy_log", &log_wrapper);
			lua.set_function("galaxy_gl_errcode_as_string", &error::gl_errcode_as_string);
			lua.set_function("galaxy_gl_get_all_errors", &error::gl_get_all_errors);
			lua.set_function("galaxy_gl_add_error", &error::gl_add_error);
		}
	} // namespace lua
} // namespace galaxy
