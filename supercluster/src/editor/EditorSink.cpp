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
		auto str    = static_cast<std::string>(message);
		auto second = str.find_first_of('[', str.find_first_of('[', 0) + 1);

		m_logs.emplace_back(str.substr(second));
	}

	const std::vector<std::string>& EditorSink::get_messages() const noexcept
	{
		return m_logs;
	}
} // namespace sc