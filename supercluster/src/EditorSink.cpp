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

	void EditorSink::sink_message(const error::LogMessage& message)
	{
		m_logs.emplace_back(message);
	}

	void EditorSink::clear()
	{
		m_logs.clear();
	}

	const meta::vector<error::LogMessage>& EditorSink::get_messages() const
	{
		return m_logs;
	}
} // namespace sc
