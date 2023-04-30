///
/// LogConsole.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_LogConsole_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_LogConsole_HPP_

#include <galaxy/graphics/Texture.hpp>

#include "editor/EditorSink.hpp"

namespace sc
{
	namespace panel
	{
		class LogConsole final
		{
		public:
			LogConsole();
			~LogConsole();

			void set_sink(EditorSink* sink);

			void render();

		private:
			EditorSink* m_sink;
			ImVec2 m_icon_size;

			galaxy::graphics::Texture m_info;
			galaxy::graphics::Texture m_warning;
			galaxy::graphics::Texture m_error;

			bool m_show_info;
			bool m_show_warning;
			bool m_show_error;
		};
	} // namespace panel
} // namespace sc

#endif