///
/// LogConsole.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/ui/ImGuiHelpers.hpp>

#include "LogConsole.hpp"

namespace sc
{
	namespace panel
	{
		LogConsole::LogConsole()
			: m_sink {nullptr}
			, m_icon_size {16, 16}
			, m_show_info {true}
			, m_show_warning {true}
			, m_show_error {true}
		{
			m_info.load("../editor_data/icons/info.png");
			m_info.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_info.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_warning.load("../editor_data/icons/warning.png");
			m_warning.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_warning.set_filter(graphics::TextureFilters::MAG_TRILINEAR);

			m_error.load("../editor_data/icons/error.png");
			m_error.set_filter(graphics::TextureFilters::MIN_TRILINEAR);
			m_error.set_filter(graphics::TextureFilters::MAG_TRILINEAR);
		}

		LogConsole::~LogConsole()
		{
			m_sink = nullptr;
		}

		void LogConsole::set_sink(EditorSink* sink)
		{
			m_sink = sink;
		}

		void LogConsole::render()
		{
			if (ImGui::Begin("Log", NULL, ImGuiWindowFlags_AlwaysVerticalScrollbar))
			{
				if (ImGui::Button("Clear"))
				{
					m_sink->clear();
				}

				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 48);

				if (ui::imgui_imagebutton(m_info, m_icon_size))
				{
					m_show_info = !m_show_info;
				}

				ImGui::SameLine();

				if (ui::imgui_imagebutton(m_warning, m_icon_size))
				{
					m_show_warning = !m_show_warning;
				}

				ImGui::SameLine();

				if (ui::imgui_imagebutton(m_error, m_icon_size))
				{
					m_show_error = !m_show_error;
				}

				constexpr const auto flags =
					ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ContextMenuInBody;

				if (ImGui::BeginTable("LogTable", 5, flags))
				{
					ImGui::TableSetupColumn("Level");
					ImGui::TableSetupColumn("Time");
					ImGui::TableSetupColumn("File");
					ImGui::TableSetupColumn("Line");
					ImGui::TableSetupColumn("Message");
					ImGui::TableHeadersRow();

					for (const auto& msg : m_sink->get_messages())
					{
						if ((msg.level == "INFO" || msg.level == "DEBUG") && !m_show_info)
						{
							continue;
						}
						else if (msg.level == "WARNING" && !m_show_warning)
						{
							continue;
						}
						else if ((msg.level == "ERROR" || msg.level == "FATAL") && !m_show_error)
						{
							continue;
						}

						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);
						ImGui::TextWrapped(msg.level.c_str());

						ImGui::TableSetColumnIndex(1);
						ImGui::TextWrapped(msg.time.c_str());

						ImGui::TableSetColumnIndex(2);
						ImGui::TextWrapped(msg.file.c_str());

						ImGui::TableSetColumnIndex(3);
						ImGui::TextWrapped(msg.line.c_str());

						ImGui::TableSetColumnIndex(4);
						ImGui::TextWrapped(msg.message.c_str());
					}

					ImGui::EndTable();
				}
			}

			ImGui::End();
		}
	} // namespace panel
} // namespace sc