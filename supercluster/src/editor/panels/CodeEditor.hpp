///
/// CodeEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_CODEEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_CODEEDITOR_HPP_

#include <filesystem>

#include <imgui_addons/TextEditor.h>

namespace sc
{
	namespace panel
	{
		class CodeEditor final
		{
		public:
			void render();

		private:
			void menu();

		public:
			TextEditor m_editor;
			std::filesystem::path m_file;
		};
	} // namespace panel
} // namespace sc

#endif