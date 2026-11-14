///
/// LogPanel.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_PANELS_LOGPANEL_HPP_
#define GALAXY_UI_PANELS_LOGPANEL_HPP_

#include "galaxy/logging/Sink.hpp"
#include "galaxy/ui/panels/Panel.hpp"
#include "galaxy/ui/ImGuiHelpers.hpp"

namespace galaxy
{
	///
	/// Sink to display log messages in overlay editor.
	///
	class EditorSink final : public Sink
	{
	public:
		///
		/// Constructor.
		///
		EditorSink() = default;

		///
		/// Destructor.
		///
		virtual ~EditorSink() noexcept;

		///
		/// Sink the message.
		///
		/// \param message Message data to send to sink.
		///
		void sink(const LogMessage& message) override;

		///
		/// Clear all logs stored.
		///
		void clear();

		///
		/// Get all stored logs.
		///
		/// \return List of all stored logs.
		///
		const std::vector<LogMessage>& get_messages() const;

	private:
		///
		/// Stored logs.
		///
		std::vector<LogMessage> m_logs;
	};

	///
	/// Panel for showing console log messages.
	///
	class LogPanel final : public Panel
	{
	public:
		///
		/// Constructor.
		///
		LogPanel() noexcept;

		///
		/// Destructor.
		///
		virtual ~LogPanel() noexcept;

		///
		/// Draw config panel.
		///
		void draw() override;

	private:
		///
		/// Bridges the panel and the log system.
		///
		EditorSink m_sink;

		///
		/// Icon size for buttons.
		///
		ImVec2 m_icon_size;

		///
		/// Level filter for info.
		///
		bool m_show_info;

		///
		/// Level filter for warning.
		///
		bool m_show_warning;

		///
		/// Level filter for error.
		///
		bool m_show_error;
	};
} // namespace galaxy

#endif
