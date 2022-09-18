///
/// LogConsole.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_LogConsole_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_LogConsole_HPP_

#include "editor/EditorSink.hpp"

namespace sc
{
	namespace panel
	{
		class LogConsole final
		{
		public:
			LogConsole() noexcept = default;
			~LogConsole() noexcept;

			void set_sink(EditorSink* sink) noexcept;

			void render();

		private:
			EditorSink* m_sink;
		};
	} // namespace panel
} // namespace sc

#endif