///
/// LogPanel.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/logging/Log.hpp"
#include "galaxy/resource/embedded/material_design_icons.h"

#include "LogPanel.hpp"

namespace galaxy
{
	EditorSink::~EditorSink() noexcept
	{
		m_logs.clear();
	}

	void EditorSink::sink(const LogMessage& message)
	{
		m_logs.emplace_back(message);
	}

	void EditorSink::clear()
	{
		m_logs.clear();
	}

	const std::vector<LogMessage>& EditorSink::get_messages() const
	{
		return m_logs;
	}

	LogPanel::LogPanel() noexcept
		: m_icon_size {32, 32}
		, m_show_info {true}
		, m_show_warning {true}
		, m_show_error {true}
	{
		GALAXY_ADD_SINK(EditorSink);
		m_show = true;
	}

	LogPanel::~LogPanel() noexcept
	{
	}

	void LogPanel::draw()
	{
		if (ImGui::Begin(ICON_MDI_POST " Log", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			if (ImGui::Button("Clear"))
			{
				m_sink.clear();
			}

			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 56);

			if (ImGui::Button(ICON_MDI_INFORMATION, m_icon_size))
			{
				m_show_info = !m_show_info;
			}

			ImGui::SameLine();

			if (ImGui::Button(ICON_MDI_ALERT, m_icon_size))
			{
				m_show_warning = !m_show_warning;
			}

			ImGui::SameLine();

			if (ImGui::Button(ICON_MDI_ALERT_CIRCLE, m_icon_size))
			{
				m_show_error = !m_show_error;
			}

			constexpr const auto flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ContextMenuInBody;

			if (ImGui::BeginTable("LogTable", 5, flags))
			{
				ImGui::TableSetupColumn("Level");
				ImGui::TableSetupColumn("Time");
				ImGui::TableSetupColumn("File");
				ImGui::TableSetupColumn("Line");
				ImGui::TableSetupColumn("Message");
				ImGui::TableHeadersRow();

				for (const auto& msg : m_sink.get_messages())
				{
					if (msg.level == LogLevel::INFO && !m_show_info)
					{
						continue;
					}
					else if (msg.level == LogLevel::WARNING && !m_show_warning)
					{
						continue;
					}
					else if ((msg.level == LogLevel::ERROR || msg.level == LogLevel::FATAL) && !m_show_error)
					{
						continue;
					}

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					std::string level {magic_enum::enum_name<LogLevel>(msg.level)};
					ImGui::TextWrapped(level.c_str());

					ImGui::TableSetColumnIndex(1);
					ImGui::TextWrapped(msg.time.c_str());

					ImGui::TableSetColumnIndex(2);
					ImGui::TextWrapped(msg.file.c_str());

					ImGui::TableSetColumnIndex(3);
					ImGui::TextWrapped(std::format("{0}", msg.line).c_str());

					ImGui::TableSetColumnIndex(4);
					ImGui::TextWrapped(msg.message.c_str());
				}

				ImGui::EndTable();
			}
		}

		ImGui::End();
	}
} // namespace galaxy
