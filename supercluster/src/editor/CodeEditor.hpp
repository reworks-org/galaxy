///
/// CodeEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_CODEEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_CODEEDITOR_HPP_

#include <filesystem>

#include <imgui_addons/TextEditor.h>

namespace sc
{
	struct CodeEditor
	{
		TextEditor m_editor;
		std::filesystem::path m_file;
	};
} // namespace sc

#endif