///
/// EditorSink.cpp
/// supercluster
///
/// See LICENSE.txt.
///

#include "EditorSink.hpp"

namespace sc
{
	EditorSink::~EditorSink() noexcept
	{
		m_logs.clear();
	}

	void EditorSink::sink_message(std::string_view message) noexcept
	{
		m_logs.emplace_back(static_cast<std::string>(message));
	}

	const std::vector<std::string>& EditorSink::get_messages() const noexcept
	{
		return m_logs;
	}
} // namespace sc