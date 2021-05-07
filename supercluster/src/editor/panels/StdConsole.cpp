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
		std::streamsize StdConsoleStream::xsputn(const char* s, std::streamsize n)
		{
			std::streamsize start = 0;
			for (std::streamsize i = 0; i < n; i++)
			{
				if (s[i] == '\n')
				{
					m_buffer.append(s + start, s + i);
					sync();
					start = i + 1;
				}
			}
			m_buffer.append(s + start, s + n);
			return n;
		}

		int StdConsoleStream::overflow(int c)
		{
			char ch = traits_type::to_char_type(c);
			if (ch == '\n')
			{
				sync();
			}
			else
			{
				m_buffer.append(1, c);
			}
			return c;
		}

		int StdConsoleStream::sync()
		{
			m_buffer.erase(0, 5);
			m_history.push_back(m_buffer);
			m_buffer.clear();

			return 0;
		}

		StdConsole::StdConsole()
		    : m_stream {static_cast<std::streambuf*>(&m_streambuf)}
		{
		}

		void StdConsole::render()
		{
			if (ImGui::Begin("Console", NULL, ImGuiWindowFlags_AlwaysVerticalScrollbar))
			{
				for (const auto& str : m_streambuf.m_history)
				{
					ImGui::TextWrapped(str.c_str());
				}

				ImGui::SetScrollHereY(1.0f);
			}

			ImGui::End();
		}

		std::ostream& StdConsole::get_stream() noexcept
		{
			return m_stream;
		}
	} // namespace panel
} // namespace sc