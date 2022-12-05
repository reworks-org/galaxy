///
/// RMLSystem.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glfw/glfw3.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/platform/Pragma.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26440)
#endif

#include "RMLSystem.hpp"

namespace galaxy
{
	namespace ui
	{
		RMLSystem::RMLSystem() noexcept
		{
			m_pointer = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
			m_cross   = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
			m_text    = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		}

		RMLSystem::~RMLSystem() noexcept
		{
			glfwDestroyCursor(m_pointer);
			glfwDestroyCursor(m_cross);
			glfwDestroyCursor(m_text);
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

			GLFWcursor* cursor = nullptr;

			if (cursor_name == "move")
			{
				cursor = m_pointer;
			}
			else if (cursor_name == "pointer")
			{
				cursor = m_pointer;
			}
			else if (cursor_name == "resize")
			{
				cursor = m_pointer;
			}
			else if (cursor_name == "cross")
			{
				cursor = m_cross;
			}
			else if (cursor_name == "text")
			{
				cursor = m_text;
			}
			else
			{
				cursor = window_cursor.m_data;
			}

			glfwSetCursor(window.handle(), cursor);
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

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif