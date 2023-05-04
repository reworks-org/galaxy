///
/// RMLSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "RMLSystem.hpp"

namespace galaxy
{
	namespace ui
	{
		RMLSystem::RMLSystem()
		{
		}

		RMLSystem::~RMLSystem()
		{
		}

		double RMLSystem::GetElapsedTime()
		{
			return glfwGetTime();
		}

		bool RMLSystem::LogMessage(Rml::Log::Type type, const Rml::String& message)
		{
			auto output = true;

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
					GALAXY_LOG(GALAXY_ERROR, "RML: {0}.", message);
					output = false;
					break;
			}

			return output;
		}

		void RMLSystem::SetMouseCursor(const Rml::String& cursor_name)
		{
			auto& window        = core::ServiceLocator<core::Window>::ref();
			auto& window_cursor = window.get_input<input::Cursor>();

			if (cursor_name == "move")
			{
				window_cursor.use_hand();
			}
			else if (cursor_name == "pointer")
			{
				window_cursor.use_pointer();
			}
			else if (cursor_name == "resize")
			{
				window_cursor.use_hand();
			}
			else if (cursor_name == "cross")
			{
				window_cursor.use_cross();
			}
			else if (cursor_name == "text")
			{
				window_cursor.use_text();
			}
			else
			{
				window_cursor.use_custom_else_pointer();
			}
		}

		void RMLSystem::SetClipboardText(const Rml::String& text)
		{
			glfwSetClipboardString(core::ServiceLocator<core::Window>::ref().handle(), text.c_str());
		}

		void RMLSystem::GetClipboardText(Rml::String& text)
		{
			text = Rml::String(glfwGetClipboardString(core::ServiceLocator<core::Window>::ref().handle()));
		}
	} // namespace ui
} // namespace galaxy