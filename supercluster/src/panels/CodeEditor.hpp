///
/// CodeEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_PANELS_CODEEDITOR_HPP_
#define SUPERCLUSTER_PANELS_CODEEDITOR_HPP_

namespace sc
{
	class CodeEditor final
	{
	  public:
		CodeEditor();
		~CodeEditor();

		void render();

	  public:
		bool m_show;
	};
} // namespace sc

#endif
