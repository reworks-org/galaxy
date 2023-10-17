///
/// EditorSink.cpp
/// supercluster
///
/// See LICENSE.txt.
///

#include "EditorSink.hpp"

namespace sc
{
	EditorSink::~EditorSink()
	{
		m_logs.clear();
	}

	void EditorSink::sink_message(std::string_view colour,
		std::string_view                           level,
		std::string_view                           time,
		std::string_view                           file,
		std::string_view                           line,
		std::string_view                           message)
	{
		// clang-format off
		m_logs.emplace_back(EditorSinkMessage
		{
			.colour  = std::string (colour),
			.level   = std::string (level),
			.time    = std::string (time),
			.file    = std::string (file),
			.line    = std::string (line),
			.message = std::string (message)
		});
		// clang-format on
	}

	void EditorSink::clear()
	{
		m_logs.clear();
	}

	const meta::vector<EditorSinkMessage>& EditorSink::get_messages() const
	{
		return m_logs;
	}
} // namespace sc
