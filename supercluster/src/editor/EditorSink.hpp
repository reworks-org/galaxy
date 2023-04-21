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
	class EditorSink final : public error::Sink
	{
	public:
		EditorSink() = default;
		virtual ~EditorSink();

		void sink_message(std::string_view message) override;

		const meta::vector<std::string>& get_messages() const;

	private:
		meta::vector<std::string> m_logs;
	};
} // namespace sc

#endif