///
/// LogConsole.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_PANELS_LOGCONSOLE_HPP_
#define SUPERCLUSTER_PANELS_LOGCONSOLE_HPP_

#include <galaxy/graphics/gl/Texture2D.hpp>

#include "../EditorSink.hpp"

namespace sc
{
	class LogConsole final
	{
	  public:
		LogConsole();
		~LogConsole();

		void set_sink(EditorSink* sink);

		void render();

	  public:
		bool m_show;

	  private:
		EditorSink* m_sink;
		ImVec2      m_icon_size;

		bool m_show_info;
		bool m_show_warning;
		bool m_show_error;
	};
} // namespace sc

#endif
