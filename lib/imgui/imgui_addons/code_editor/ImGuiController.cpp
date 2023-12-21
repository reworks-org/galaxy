#include "ImGuiController.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <imgui.h>
#include <imgui_addons/material_design_icons.h>
#include <imgui_internal.h>

#include "portable-file-dialogs.h"

#include "FontManager.h"
#include "panels/DirectoryFinder.h"
#include "panels/DirectoryTreeView.h"
#include "panels/FileTextEdit.h"
#include "PathUtils.h"
#include "Utils.h"

#include <GLFW/glfw3.h>

namespace scex::ImGuiController
{
	int folderViewForLastFocusedPanel = -1;

	ImGuiID leftDockID  = -1;
	ImGuiID rightDockID = -1;

	// ---- Callback declarations ---- //
	void OnFolderViewNodeFound();
	void OnFolderOpenInFileExplorer(const std::string& folderPath, int folderViewId);
	void OnFindInFolder(const std::string& folderPath, int folderViewId);
	void OnShowInFileExplorer(const std::string& filePath, int folderViewId);
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

	std::vector<std::pair<std::string, DirectoryTreeView::OnContextMenuCallback>> folderViewFileContextMenuOptions = {
		{"Show in file explorer", OnShowInFileExplorer}
    };
	std::vector<std::pair<std::string, DirectoryTreeView::OnContextMenuCallback>> folderViewFolderContextMenuOptions = {
		{       "Find in folder",             OnFindInFolder},
		{"Show in file explorer",       OnShowInFileExplorer},
		{"Open in file explorer", OnFolderOpenInFileExplorer}
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

	// ---- Callbacks from folder view ---- //
	void OnFolderViewNodeFound()
	{
		glfwPostEmptyEvent();
	}

	void OnFolderOpenInFileExplorer(const std::string& folderPath, int folderViewId)
	{
		// doesn't work with non ASCII
		std::string command = "explorer \"" + folderPath + "\"";
		system(command.c_str());
	}

	void OnFindInFolder(const std::string& folderPath, int folderViewId)
	{
		CreateNewFolderSearch(folderPath, folderViewId);
	}

	void OnShowInFileExplorer(const std::string& filePath, int folderViewId)
	{
		auto        path             = std::filesystem::path(filePath);
		std::string parentFolderPath = path.parent_path().string();
		// doesn't work with non ASCII
		std::string command = "explorer /select,\"" + path.string() + "\",\"" + parentFolderPath + "\"";
		system(command.c_str());
	}

	void OnFileClickedInFolderView(const std::string& filePath, int folderViewId)
	{
		if (fileToEditorMap.find(filePath) == fileToEditorMap.end() || fileToEditorMap[filePath] == nullptr)
			fileToEditorMap[filePath] = CreateNewEditor(filePath.c_str(), folderViewId);
		else
			editorToFocus = fileToEditorMap[filePath];
	}

	// ---- Callbacks from file text edit ---- //
	void OnShowInFolderView(const std::string& filePath, int folderViewId)
	{
		assert(folderViewId > -1);
		assert(folderViewers[folderViewId] != nullptr);
		folderViewers[folderViewId]->ShowFile(filePath);
	}

	// ---- Callbacks from folder finder ---- //
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

	// ---- Generic Callbacks ---- //
	void OnPanelFocused(int folderViewId)
	{
		folderViewForLastFocusedPanel = folderViewId;
	}
} // namespace scex::ImGuiController

void scex::ImGuiController::Setup(const std::string& root)
{
	PathUtils::SetProgramDirectory(root);

	int fontSize = 17;
	FileTextEdit::SetDarkPaletteAsDefault();

	FontManager::Initialize(ImGui::GetIO(), fontSize);
}

bool scex::ImGuiController::HasControl()
{
	return ImGui::GetIO().WantCaptureMouse;
}

void scex::ImGuiController::OnPathsDropped(const char** paths, int pathCount)
{
	for (int i = 0; i < pathCount; i++)
	{
		std::filesystem::path pathObject(paths[i]);
		if (std::filesystem::is_directory(pathObject))
			CreateNewFolderViewer(paths[i]);
		else
			CreateNewEditor(paths[i]);
	}
}

void scex::ImGuiController::Tick(double deltaTime)
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

		bool ctrlKeyDown = ImGui::GetIO().KeyCtrl;
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F1)))
			menuBarEnabled = !menuBarEnabled;

		newTextPanelRequested |= ctrlKeyDown && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_N), false);
		openFileRequested     |= ctrlKeyDown && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_O), false);
		fileSearchRequested   |= ctrlKeyDown && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_P), false);

		if (menuBarEnabled)
		{
			if (ImGui::BeginMenuBar())
			{
				newTextPanelRequested |= ImGui::MenuItem("New text panel", "Ctrl+N");
				openFileRequested     |= ImGui::MenuItem("Open file", "Ctrl+O");
				openFolderRequested   |= ImGui::MenuItem("Open folder");

				ImGui::EndMenuBar();
			}
		}
		if (newTextPanelRequested)
			CreateNewEditor();
		else if (openFileRequested)
		{
			std::vector<std::string> selection = pfd::open_file("Open file", "", {"Any file", "*"}).result();
			if (selection.size() > 0) // if not canceled
				fileToEditorMap[selection[0]] = CreateNewEditor(selection[0].c_str());
		}
		else if (openFolderRequested)
		{
			std::string folder = pfd::select_folder("Open folder").result();
			if (folder.length() > 0) // if not canceled
				CreateNewFolderViewer(folder);
		}
		else if (fileSearchRequested)
		{
			if (folderViewForLastFocusedPanel >= 0 && folderViewers[folderViewForLastFocusedPanel] != nullptr)
				folderViewers[folderViewForLastFocusedPanel]->RunSearch();
		}

		{
			int folderViewToDelete = -1;
			for (int i = 0; i < folderViewers.size(); i++)
			{
				DirectoryTreeView* folderView = folderViewers[i];
				if (folderView == nullptr)
					continue;
				ImGui::SetNextWindowDockID(leftDockID, ImGuiCond_FirstUseEver);
				if (!folderView->OnImGui(deltaTime))
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

void scex::ImGuiController::Load(const std::string& file)
{
	CreateNewEditor(file.c_str());
}
