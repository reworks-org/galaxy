///
/// EditorSink.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_EDITORSINK_HPP_
#define SUPERCLUSTER_EDITOR_EDITORSINK_HPP_

#include <vector>

#include <galaxy/error/Sink.hpp>
#include <galaxy/meta/Memory.hpp>

using namespace galaxy;

namespace sc
{
	struct EditorSinkMessage
	{
		std::string colour;
		std::string level;
		std::string time;
		std::string file;
		std::string line;
		std::string message;
	};

	class EditorSink final : public error::Sink
	{
	public:
		EditorSink() = default;
		virtual ~EditorSink();

		void sink_message(std::string_view colour,
			std::string_view level,
			std::string_view time,
			std::string_view file,
			std::string_view line,
			std::string_view message) override;
		void clear();

		const meta::vector<EditorSinkMessage>& get_messages() const;

	private:
		meta::vector<EditorSinkMessage> m_logs;
	};
} // namespace sc

#endif