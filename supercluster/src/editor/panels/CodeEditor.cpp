///
/// CodeEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <imgui_addons/material_design_icons.h>

#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/fs/VirtualFileSystem.hpp>
#include <galaxy/ui/ImGuiHelpers.hpp>

#include "CodeEditor.hpp"

using namespace galaxy;

namespace sc
{
	namespace panel
	{
		void CodeEditor::render()
		{
			if (ImGui::Begin(ICON_MDI_CODE_ARRAY " Code Editor", nullptr, ImGuiWindowFlags_MenuBar))
			{
				if (ImGui::BeginMenuBar())
				{
					menu();

					ImGui::EndMenuBar();
				}

				ui::imgui_confirm("NewCodeEditorFile", [&]() {
					m_editor.SetText("");
					m_file = "";
				});

				const auto cpos = m_editor.GetCursorPosition();
				ImGui::Text("%6d/%-6d %6d lines | %s%s",
					cpos.mLine + 1,
					cpos.mColumn + 1,
					m_editor.GetTotalLines(),
					m_file.empty() ? "New File" : m_file.string().c_str(),
					m_editor.CanUndo() ? "*" : "");

				m_editor.Render("Editor");
				if (ImGui::BeginDragDropTarget())
				{
					if (const auto payload = ImGui::AcceptDragDropPayload("AssetPanelItem"))
					{
						const char* path = static_cast<const char*>(payload->Data);
						const auto data  = core::ServiceLocator<fs::VirtualFileSystem>::ref().open(path);

						if (!data.empty())
						{
							m_editor.SetText(data);
							m_file = path;
						}
						else
						{
							ui::imgui_notify_error("Failed to open script.");
						}
					}

					ImGui::EndDragDropTarget();
				}

				if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("CodeEditorContextMenu");
				}

				if (ImGui::BeginPopupContextWindow("CodeEditorContextMenu"))
				{
					menu();
					ImGui::EndPopup();
				}
			}

			ImGui::End();
		}

		void CodeEditor::menu()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					if (!m_editor.GetText().empty())
					{
						ui::imgui_open_confirm("NewCodeEditorFile");
					}
					else
					{
						m_file = "";
					}
				}

				if (ImGui::MenuItem("Open"))
				{
					auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

					const auto path = fs.open_file_dialog({"*.lua"}, "scripts/");
					const auto data = fs.open(path);

					if (!path.empty() && !data.empty())
					{
						m_editor.SetText(data);
						m_file = path;
					}
					else
					{
						ui::imgui_notify_error("Failed to open script.");
					}
				}

				if (ImGui::MenuItem("Save"))
				{
					auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

					if (m_file.empty())
					{
						const auto path = fs.open_save_dialog("untitled.lua", {"*.lua"});

						if (!path.empty())
						{
							m_file = path;
						}
					}

					if (!fs.save(m_editor.GetText(), m_file.string()))
					{
						ui::imgui_notify_error("Failed to save script.");
					}
				}

				if (ImGui::MenuItem("Save as..."))
				{
					if (!core::ServiceLocator<fs::VirtualFileSystem>::ref().save_using_dialog(m_editor.GetText(), "untitled.lua", {"*.lua"}))
					{
						ui::imgui_notify_error("Failed to save script.");
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				auto ro = m_editor.IsReadOnly();
				if (ImGui::MenuItem("Read only", nullptr, &ro))
				{
					m_editor.SetReadOnly(ro);
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "Ctrl+Z | Alt+Backspace", nullptr, !ro && m_editor.CanUndo()))
				{
					m_editor.Undo();
				}

				if (ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, !ro && m_editor.CanRedo()))
				{
					m_editor.Redo();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl+C", nullptr, m_editor.HasSelection()))
				{
					m_editor.Copy();
				}

				if (ImGui::MenuItem("Cut", "Ctrl+X", nullptr, !ro && m_editor.HasSelection()))
				{
					m_editor.Cut();
				}

				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && m_editor.HasSelection()))
				{
					m_editor.Delete();
				}

				if (ImGui::MenuItem("Paste", "Ctrl+V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				{
					m_editor.Paste();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", "Ctrl+A", nullptr))
				{
					m_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(m_editor.GetTotalLines(), 0));
				}

				ImGui::EndMenu();
			}
		}
	} // namespace panel
} // namespace sc