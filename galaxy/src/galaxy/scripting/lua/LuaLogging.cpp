///
/// LuaError.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/logging/Log.hpp"

namespace galaxy
{
	namespace lua
	{
		void log_wrapper(logging::LogLevel error_level, std::string_view message)
		{
			switch (error_level)
			{
				case logging::LogLevel::_INFO_:
					GALAXY_LOG(GALAXY_INFO, "{0}", message);
					break;

				case logging::LogLevel::_DEBUG_:
					GALAXY_LOG(GALAXY_DEBUG, "{0}", message);
					break;

				case logging::LogLevel::_WARNING_:
					GALAXY_LOG(GALAXY_WARNING, "{0}", message);
					break;

				case logging::LogLevel::_ERROR_:
					GALAXY_LOG(GALAXY_ERROR, "{0}", message);
					break;

				case logging::LogLevel::_FATAL_:
					GALAXY_LOG(GALAXY_FATAL, "{0}", message);
					break;
			}
		}

		void inject_logging()
		{
			auto& lua = entt::locator<sol::state>::value();

			// clang-format off
			lua.new_enum<logging::LogLevel>("LogLevels",
			{
				{"INFO", logging::LogLevel::_INFO_},
				{"DEBUG", logging::LogLevel::_DEBUG_},
				{"WARNING", logging::LogLevel::_WARNING_},
				{"ERROR", logging::LogLevel::_ERROR_},
				{"FATAL", logging::LogLevel::_FATAL_}
			});
			// clang-format on

			lua.set_function("galaxy_log", &log_wrapper);
		}
	} // namespace lua
} // namespace galaxy
