///
/// StdConsole.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_STDCONSOLE_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_STDCONSOLE_HPP_

#include "editor/EditorSink.hpp"

namespace sc
{
	namespace panel
	{
		class StdConsole final
		{
		public:
			StdConsole() noexcept = default;
			~StdConsole() noexcept;

			void set_sink(EditorSink* sink) noexcept;

			void render();

		private:
			EditorSink* m_sink;
		};
	} // namespace panel
} // namespace sc

#endif