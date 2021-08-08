///
/// ScriptEditor.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_SCRIPTEDITOR_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_SCRIPTEDITOR_HPP_

#include <imgui/addons/TextEditor.h>

namespace sc
{
	namespace panel
	{
		class ScriptEditor final
		{
		public:
			ScriptEditor() noexcept;
			~ScriptEditor() noexcept = default;

			void render();

		private:
			ImGui::TextEditor m_editor;
		};
	} // namespace panel
} // namespace sc

#endif