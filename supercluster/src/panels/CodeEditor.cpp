///
/// CodeEditor.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <GLFW/glfw3.h>

#include <imgui/imgui_internal.h>
#include <imgui/imnotify/material_design_icons.h>
#include <imgui/text_edit/FontManager.h>
#include <imgui/text_edit/panels/DirectoryFinder.h>
#include <imgui/text_edit/panels/DirectoryTreeView.h>
#include <imgui/text_edit/panels/FileTextEdit.h>
#include <imgui/text_edit/portable-file-dialogs.h>

#include <galaxy/utils/Globals.hpp>

#include "CodeEditor.hpp"

namespace sc
{
	int folderViewForLastFocusedPanel = -1;

	ImGuiID leftDockID  = -1;
	ImGuiID rightDockID = -1;

	void OnFolderViewNodeFound();
	void OnFindInFolder(const std::string& folderPath, int folderViewId);
	void OnFileClickedInFolderView(const std::string& filePath, int folderViewId);
	void OnShowInFolderView(const std::string& filePath, int folderViewId);
	void OnFolderSearchResultClick(const std::string& filePath, const DirectoryFinderSearchResult& searchResult, int folderViewId);
	void OnFolderSearchResultFoundOrSearchFinished();
	void OnPanelFocused(int folderViewId);

	bool menuBarEnabled    = true;
	bool textEditDebugInfo = false;

	ImFont* textEditorFont;
	ImFont* uiFont;

	std::unordered_map<std::string, FileTextEdit*> fileToEditorMap;
	FileTextEdit*                                  editorToFocus = nullptr;

	std::vector<DirectoryFinder*>   folderFinders;
	std::vector<DirectoryTreeView*> folderViewers;
	std::vector<FileTextEdit*>      fileTextEdits;

	std::vector<std::pair<std::string, DirectoryTreeView::OnContextMenuCallback>> folderViewFileContextMenuOptions   = {};
	std::vector<std::pair<std::string, DirectoryTreeView::OnContextMenuCallback>> folderViewFolderContextMenuOptions = {
		{"Find in folder", OnFindInFolder},
	};

	FileTextEdit* CreateNewEditor(const char* filePath = nullptr, int fromFolderView = -1)
	{
		int fileTextEditId = fileTextEdits.size();
		fileTextEdits.push_back(new FileTextEdit(filePath, fileTextEditId, fromFolderView, OnPanelFocused, OnShowInFolderView));
		return fileTextEdits.back();
	}

	void CreateNewFolderViewer(const std::string& folderPath)
	{
		int folderViewerId = folderViewers.size();
		folderViewers.push_back(new DirectoryTreeView(folderPath,
			OnFolderViewNodeFound,
			OnFileClickedInFolderView,
			OnPanelFocused,
			&folderViewFileContextMenuOptions,
			&folderViewFolderContextMenuOptions,
			folderViewerId));
	}

	void CreateNewFolderSearch(const std::string& folderPath, int fromFolderView)
	{
		int folderSearchId = folderFinders.size();
		folderFinders.push_back(new DirectoryFinder(folderPath,
			OnFolderSearchResultClick,
			OnFolderSearchResultFoundOrSearchFinished,
			OnFolderSearchResultFoundOrSearchFinished,
			OnPanelFocused,
			folderSearchId,
			fromFolderView));
	}

	void InitializeLayout(ImGuiID dock_main_id)
	{
		leftDockID  = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.23f, NULL, &dock_main_id);
		rightDockID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.3f, NULL, &dock_main_id);
		ImGui::DockBuilderFinish(dock_main_id);
	}

	void OnFolderViewNodeFound()
	{
		glfwPostEmptyEvent();
	}

	void OnFindInFolder(const std::string& folderPath, int folderViewId)
	{
		CreateNewFolderSearch(folderPath, folderViewId);
	}

	void OnFileClickedInFolderView(const std::string& filePath, int folderViewId)
	{
		if (fileToEditorMap.find(filePath) == fileToEditorMap.end() || fileToEditorMap[filePath] == nullptr)
			fileToEditorMap[filePath] = CreateNewEditor(filePath.c_str(), folderViewId);
		else
			editorToFocus = fileToEditorMap[filePath];
	}

	void OnShowInFolderView(const std::string& filePath, int folderViewId)
	{
		assert(folderViewId > -1);
		assert(folderViewers[folderViewId] != nullptr);
		folderViewers[folderViewId]->ShowFile(filePath);
	}

	void OnFolderSearchResultClick(const std::string& filePath, const DirectoryFinderSearchResult& searchResult, int folderViewId)
	{
		FileTextEdit* targetEditor;
		if (fileToEditorMap.find(filePath) == fileToEditorMap.end() || fileToEditorMap[filePath] == nullptr)
			targetEditor = fileToEditorMap[filePath] = CreateNewEditor(filePath.c_str(), folderViewId);
		else
			targetEditor = editorToFocus = fileToEditorMap[filePath];
		targetEditor->SetSelection(searchResult.lineNumber - 1, searchResult.startCharIndex, searchResult.lineNumber - 1, searchResult.endCharIndex);
		targetEditor->CenterViewAtLine(searchResult.lineNumber - 1);
	}

	void OnFolderSearchResultFoundOrSearchFinished()
	{
		glfwPostEmptyEvent();
	}

	void OnPanelFocused(int folderViewId)
	{
		folderViewForLastFocusedPanel = folderViewId;
	}

	CodeEditor::CodeEditor()
		: m_show {true}
	{
		FileTextEdit::SetDarkPaletteAsDefault();
		FontManager::Initialize(ImGui::GetIO(), 17);
	}

	CodeEditor::~CodeEditor()
	{
	}

	void CodeEditor::render()
	{
		if (m_show)
		{
			if (ImGui::Begin(ICON_MDI_CODE_ARRAY " Code Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar))
			{
				ImGuiID mainDockID = ImGui::DockSpace(ImGui::GetID("DockSpace"));
				if (leftDockID == -1)
					InitializeLayout(mainDockID);

				bool newTextPanelRequested = false;
				bool openFileRequested     = false;
				bool openFolderRequested   = false;
				bool fileSearchRequested   = false;

				if (ImGui::BeginMenuBar())
				{
					newTextPanelRequested |= ImGui::MenuItem("New Editor");
					openFileRequested     |= ImGui::MenuItem("Open File");
					openFolderRequested   |= ImGui::MenuItem("Open Folder");

					ImGui::EndMenuBar();
				}

				if (newTextPanelRequested)
				{
					CreateNewEditor();
				}
				else if (openFileRequested)
				{
					std::vector<std::string> selection = pfd::open_file("Open file", "", {"Any file", "*"}).result();
					if (selection.size() > 0)
					{
						fileToEditorMap[selection[0]] = CreateNewEditor(selection[0].c_str());
					}
				}
				else if (openFolderRequested)
				{
					std::string folder = pfd::select_folder("Open folder").result();
					if (folder.length() > 0)
					{
						CreateNewFolderViewer(folder);
					}
				}
				else if (fileSearchRequested)
				{
					if (folderViewForLastFocusedPanel >= 0 && folderViewers[folderViewForLastFocusedPanel] != nullptr)
					{
						folderViewers[folderViewForLastFocusedPanel]->RunSearch();
					}
				}

				{
					int folderViewToDelete = -1;
					for (int i = 0; i < folderViewers.size(); i++)
					{
						DirectoryTreeView* folderView = folderViewers[i];
						if (folderView == nullptr)
							continue;
						ImGui::SetNextWindowDockID(leftDockID, ImGuiCond_FirstUseEver);
						if (!folderView->OnImGui(GALAXY_DT))
							folderViewToDelete = i;
					}
					if (folderViewToDelete > -1)
					{
						delete folderViewers[folderViewToDelete];
						folderViewers[folderViewToDelete] = nullptr;
						// notify text editors so they don't try to show file in folder view
						for (int i = 0; i < fileTextEdits.size(); i++)
						{
							FileTextEdit* fte = fileTextEdits[i];
							if (fte == nullptr)
								continue;
							fte->OnFolderViewDeleted(folderViewToDelete);
						}
					}
				}
				{
					int finderToDelete = -1;
					for (int i = 0; i < folderFinders.size(); i++)
					{
						DirectoryFinder* finder = folderFinders[i];
						if (finder == nullptr)
							continue;
						ImGui::SetNextWindowDockID(rightDockID, ImGuiCond_FirstUseEver);
						if (!finder->OnImGui())
							finderToDelete = i;
					}
					if (finderToDelete > -1)
					{
						delete folderFinders[finderToDelete];
						folderFinders[finderToDelete] = nullptr;
					}
				}
				{
					int fileTextEditToDelete = -1;
					for (int i = 0; i < fileTextEdits.size(); i++)
					{
						FileTextEdit* fte = fileTextEdits[i];
						if (fte == nullptr)
							continue;
						ImGui::SetNextWindowDockID(mainDockID, ImGuiCond_FirstUseEver);
						if (editorToFocus == fte)
						{
							ImGui::SetNextWindowFocus();
							editorToFocus = nullptr;
						}
						if (!fte->OnImGui())
							fileTextEditToDelete = i;
					}
					if (fileTextEditToDelete > -1)
					{
						const char* associatedFile = fileTextEdits[fileTextEditToDelete]->GetAssociatedFile();
						if (associatedFile != nullptr)
							fileToEditorMap.erase(associatedFile);
						delete fileTextEdits[fileTextEditToDelete];
						fileTextEdits[fileTextEditToDelete] = nullptr;
					}
				}
			}

			ImGui::End();
		}
	}

	void CodeEditor::load(const std::string& file)
	{
		fileToEditorMap[file] = CreateNewEditor(file.c_str());
	}
} // namespace sc
