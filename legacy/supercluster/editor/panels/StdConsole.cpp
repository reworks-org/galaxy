///
/// StdConsole.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui/imgui_stdlib.h>

#include "StdConsole.hpp"

namespace sc
{
	namespace panel
	{
		StdConsole::~StdConsole() noexcept
		{
			m_sink = nullptr;
		}

		void StdConsole::set_sink(EditorSink* sink) noexcept
		{
			m_sink = sink;
		}

		void StdConsole::render()
		{
			if (ImGui::Begin("Console", NULL, ImGuiWindowFlags_AlwaysVerticalScrollbar))
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