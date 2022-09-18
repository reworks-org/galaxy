///
/// LogConsole.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui_stdlib.h>

#include "LogConsole.hpp"

namespace sc
{
	namespace panel
	{
		LogConsole::~LogConsole() noexcept
		{
			m_sink = nullptr;
		}

		void LogConsole::set_sink(EditorSink* sink) noexcept
		{
			m_sink = sink;
		}

		void LogConsole::render()
		{
			if (ImGui::Begin("Log", NULL, ImGuiWindowFlags_AlwaysVerticalScrollbar))
			{
				for (const auto& str : m_sink->get_messages())
				{
					ImGui::TextWrapped(str.c_str());
				}

				ImGui::SetScrollHereY(1.0f);
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc