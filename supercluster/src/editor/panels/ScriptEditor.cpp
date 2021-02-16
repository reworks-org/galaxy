///
/// ScriptEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/FileSystem.hpp>

#include "ScriptEditor.hpp"

namespace sc
{
	namespace panel
	{
		ScriptEditor::ScriptEditor()
		{
			m_editor.SetLanguageDefinition(ImGui::TextEditor::LanguageDefinition::Lua());
		}

		void ScriptEditor::render(bool* show)
		{
			if (*show)
			{
				if (ImGui::Begin("Script Editor", show, ImGuiWindowFlags_MenuBar))
				{
					ImGui::SetWindowSize({800, 600}, ImGuiCond_FirstUseEver);

					if (ImGui::BeginMenuBar())
					{
						if (ImGui::BeginMenu("File"))
						{
							if (ImGui::MenuItem("Open"))
							{
								const auto code = SL_HANDLE.vfs()->open_with_dialog("*.lua");
								m_editor.SetText(code);
							}

							if (ImGui::MenuItem("Save"))
							{
								SL_HANDLE.vfs()->save_with_dialog(m_editor.GetText());
							}

							if (ImGui::MenuItem("Close"))
							{
								*show = false;
							}

							ImGui::EndMenu();
						}

						if (ImGui::BeginMenu("Edit"))
						{
							bool ro = m_editor.IsReadOnly();
							if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
							{
								m_editor.SetReadOnly(ro);
							}

							ImGui::Separator();

							if (ImGui::MenuItem("Undo", "", nullptr, !ro && m_editor.CanUndo()))
							{
								m_editor.Undo();
							}

							if (ImGui::MenuItem("Redo", "", nullptr, !ro && m_editor.CanRedo()))
							{
								m_editor.Redo();
							}

							ImGui::Separator();

							if (ImGui::MenuItem("Copy", "", nullptr, m_editor.HasSelection()))
							{
								m_editor.Copy();
							}

							if (ImGui::MenuItem("Cut", "", nullptr, !ro && m_editor.HasSelection()))
							{
								m_editor.Cut();
							}

							if (ImGui::MenuItem("Delete", "", nullptr, !ro && m_editor.HasSelection()))
							{
								m_editor.Delete();
							}

							if (ImGui::MenuItem("Paste", "", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
							{
								m_editor.Paste();
							}

							ImGui::Separator();

							if (ImGui::MenuItem("Select all", nullptr, nullptr))
							{
								m_editor.SetSelection(ImGui::TextEditor::Coordinates(), ImGui::TextEditor::Coordinates(m_editor.GetTotalLines(), 0));
							}

							ImGui::EndMenu();
						}

						if (ImGui::BeginMenu("Theme"))
						{
							if (ImGui::MenuItem("Dark palette"))
							{
								m_editor.SetPalette(ImGui::TextEditor::GetDarkPalette());
							}

							if (ImGui::MenuItem("Light palette"))
							{
								m_editor.SetPalette(ImGui::TextEditor::GetLightPalette());
							}

							if (ImGui::MenuItem("Retro blue palette"))
							{
								m_editor.SetPalette(ImGui::TextEditor::GetRetroBluePalette());
							}

							ImGui::EndMenu();
						}

						ImGui::EndMenuBar();
					}

					m_editor.Render("Script Editor");
				}

				ImGui::End();
			}
		}
	} // namespace panel
} // namespace sc