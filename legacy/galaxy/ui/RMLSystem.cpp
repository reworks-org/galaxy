///
/// RMLSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/error/Log.hpp"

#include "RMLSystem.hpp"

namespace galaxy
{
	namespace ui
	{
		double RMLSystem::GetElapsedTime()
		{
			const auto now     = std::chrono::high_resolution_clock::now();
			const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now - SL_HANDLE.m_app_start_time_point);

			return std::chrono::duration<double>(seconds).count();
		}

		bool RMLSystem::LogMessage(Rml::Log::Type type, const Rml::String& message)
		{
			bool output = true;

			switch (type)
			{
				case Rml::Log::Type::LT_INFO:
					GALAXY_LOG(GALAXY_INFO, "RML: {0}.", message);
					break;

				case Rml::Log::Type::LT_DEBUG:
					GALAXY_LOG(GALAXY_DEBUG, "RML: {0}.", message);
					break;

				case Rml::Log::Type::LT_ALWAYS:
				case Rml::Log::Type::LT_MAX:
				case Rml::Log::Type::LT_WARNING:
					GALAXY_LOG(GALAXY_WARNING, "RML: {0}.", message);
					break;

				case Rml::Log::Type::LT_ERROR:
					GALAXY_LOG(GALAXY_ERROR, "RML: {0}.", message);
					break;

				case Rml::Log::Type::LT_ASSERT:
					GALAXY_LOG(GALAXY_FATAL, "RML: {0}.", message);
					output = false;
					break;
			}

			return output;
		}

		void RMLSystem::SetMouseCursor(const Rml::String& cursor_name)
		{
			SL_HANDLE.window()->set_cursor_icon(cursor_name);
		}

		void RMLSystem::SetClipboardText(const Rml::String& text)
		{
			glfwSetClipboardString(SL_HANDLE.window()->gl_window(), text.c_str());
		}

		void RMLSystem::GetClipboardText(Rml::String& text)
		{
			text = glfwGetClipboardString(SL_HANDLE.window()->gl_window());
		}
	} // namespace ui
} // namespace galaxy