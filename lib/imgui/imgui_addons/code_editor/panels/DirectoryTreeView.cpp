#include "DirectoryTreeView.h"

#include <iostream>

#define MAX_SEARCH_RESULTS 50

DirectoryTreeView::DirectoryTreeView(const std::string& folderPath,
	OnFileClickCallback fileClickCallback,
	OnFocusedCallback onFocusedCallback,
	std::vector<std::pair<std::string, OnContextMenuCallback>>* fileContextMenuOptions,
	std::vector<std::pair<std::string, OnContextMenuCallback>>* folderContextMenuOptions,
	int id)
{
	this->id                       = id;
	panelName                      = "Folder view##" + std::to_string((int)this);
	directoryPath                  = folderPath;
	this->fileClickCallback        = fileClickCallback;
	this->onFocusedCallback        = onFocusedCallback;
	this->fileContextMenuOptions   = fileContextMenuOptions;
	this->folderContextMenuOptions = folderContextMenuOptions;
	findFilesBuffer[0]             = '\0';
	directoryTreeRoot              = CreateDirectoryNodeTreeFromPath(directoryPath);
}

DirectoryTreeView::~DirectoryTreeView()
{
	for (auto& item : searchTrie.children)
		Trie::Free(item.second);
}

bool DirectoryTreeView::OnImGui()
{
	bool windowIsOpen = true;
	if (requestingFocus)
		ImGui::SetNextWindowFocus();
	if (ImGui::Begin(panelName.c_str(), &windowIsOpen, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Search"))
			{
				RunSearch();
			}

			ImGui::EndMenuBar();
		}

		if (ImGui::IsWindowFocused() && onFocusedCallback != nullptr)
			onFocusedCallback(this->id);

		if (searching)
		{
			if (ImGui::IsKeyDown(ImGuiKey_Escape))
				searching = false;

			if (requestingFocus)
			{
				ImGui::SetKeyboardFocusHere();
				requestingFocus = false;
			}
			if (ImGui::InputText("Find files", findFilesBuffer, FIND_FILES_BUFFER_SIZE)) // true when text changes
			{
				searchResults.clear();
				Trie::GetSuggestions(&searchTrie, std::string(findFilesBuffer), searchResults, MAX_SEARCH_RESULTS);
			}
			bool searchTextFieldHasFocus = ImGui::IsItemFocused();
			bool pressedEnterOnSearchbarAndThereAreSearchResults =
				searchTextFieldHasFocus && searchResults.size() > 0 &&
				(ImGui::IsKeyDown(ImGuiKey_Enter) ||
					ImGui::IsKeyDown(ImGuiKey_KeypadEnter)); // to be able to press enter right after typing without using arrow keys

			bool callbackCalled = false;
			for (const std::string& searchResult : searchResults)
			{
				if (callbackCalled)
					break;
				bool fileNameIsUnique = fileNameToPath[searchResult].size() == 1;
				for (const std::string& filePath : fileNameToPath[searchResult])
				{
					if (fileClickCallback != nullptr)
					{
						if ((fileNameIsUnique ? ImGui::Selectable(searchResult.c_str()) : ImGui::Selectable((searchResult + " (" + filePath + ")").c_str())) ||
							pressedEnterOnSearchbarAndThereAreSearchResults ||
							ImGui::IsItemFocused() && (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)))
						{
							fileClickCallback(filePath, id);
							ImGui::GetIO().ClearInputKeys();
							searching      = false;
							callbackCalled = true;
							break;
						}
					}
				}
			}
			ImGui::Separator();
		}

		isHoveringNodeThisFrame = false;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0.0f, 0.0f});
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0.0f, 2.0f});
		ImGui::SetNextItemOpen(true);
		RecursivelyDisplayDirectoryNode(directoryTreeRoot);
		ImGui::PopStyleVar(2);

		if (lastHoveredNode != nullptr)
		{
			const auto* vectorToUse = lastHoveredNode->isDirectory ? folderContextMenuOptions : fileContextMenuOptions;
			if (ImGui::IsMouseDown(1) && isHoveringNodeThisFrame)
			{
				if (vectorToUse->size() > 0)
					ImGui::OpenPopup(lastHoveredNode->isDirectory ? "folder_right_click_popup" : "file_right_click_popup");
			}
			if (vectorToUse->size() > 0 && ImGui::BeginPopup(lastHoveredNode->isDirectory ? "folder_right_click_popup" : "file_right_click_popup"))
			{
				if (lastHoveredNode == &directoryTreeRoot && ImGui::Selectable("Refresh folder"))
				{
					Refresh();
					ImGui::CloseCurrentPopup();
				}
				for (auto& item : *vectorToUse)
				{
					if (ImGui::Selectable(item.first.c_str()))
					{
						item.second(lastHoveredNode->fullPath, id);
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}
		}
	}
	ImGui::End();
	return windowIsOpen;
}

void DirectoryTreeView::RunSearch()
{
	requestingFocus = searching = true;
	findFilesBuffer[0]          = '\0';
	searchResults.clear();
}

void DirectoryTreeView::Refresh()
{
	for (auto& item : searchTrie.children)
		Trie::Free(item.second);
	searchTrie.isWordEnd = false;
	searchTrie.children.clear();
	fileNameToPath.clear();
	searchResults.clear();
	directoryTreeRoot = CreateDirectoryNodeTreeFromPath(directoryPath);
}

void DirectoryTreeView::RecursivelyAddDirectoryNodes(DirectoryTreeViewNode& parentNode, std::filesystem::directory_iterator directoryIterator)
{
	for (const std::filesystem::directory_entry& entry : directoryIterator)
	{
		DirectoryTreeViewNode& childNode = parentNode.children.emplace_back();
		childNode.fullPath               = entry.path().string();
		childNode.fileName               = entry.path().filename().string();
		if (childNode.isDirectory = entry.is_directory(); childNode.isDirectory)
			RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
		else
		{
			Trie::Insert(&searchTrie, childNode.fileName);
			fileNameToPath[childNode.fileName].push_back(childNode.fullPath);
		}
	}

	auto moveDirectoriesToFront = [](const DirectoryTreeViewNode& a, const DirectoryTreeViewNode& b) {
		return (a.isDirectory > b.isDirectory);
	};
	std::sort(parentNode.children.begin(), parentNode.children.end(), moveDirectoriesToFront);
}

DirectoryTreeViewNode DirectoryTreeView::CreateDirectoryNodeTreeFromPath(const std::filesystem::path& rootPath)
{
	DirectoryTreeViewNode rootNode;
	rootNode.fullPath = rootPath.string();
	rootNode.fileName = rootPath.filename().string();
	if (rootNode.isDirectory = std::filesystem::is_directory(rootPath); rootNode.isDirectory)
		RecursivelyAddDirectoryNodes(rootNode, std::filesystem::directory_iterator(rootPath));

	return rootNode;
}

void DirectoryTreeView::SetLastHoveredNode(const DirectoryTreeViewNode* node)
{
	lastHoveredNode         = node;
	isHoveringNodeThisFrame = true;
}

void DirectoryTreeView::RecursivelyDisplayDirectoryNode(const DirectoryTreeViewNode& parentNode)
{
	ImGui::PushID(&parentNode);
	if (parentNode.isDirectory)
	{
		if (ImGui::TreeNodeEx(parentNode.fileName.c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding))
		{
			if (ImGui::IsItemHovered())
				SetLastHoveredNode(&parentNode);
			for (const DirectoryTreeViewNode& childNode : parentNode.children)
				RecursivelyDisplayDirectoryNode(childNode);
			ImGui::TreePop();
		}
		else
		{
			if (ImGui::IsItemHovered())
				SetLastHoveredNode(&parentNode);
		}
	}
	else
	{
		if (ImGui::TreeNodeEx(parentNode.fileName.c_str(),
				ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding))
		{
			if (ImGui::IsItemClicked(0))
			{
				if (fileClickCallback != nullptr)
					fileClickCallback(parentNode.fullPath, id);
			}
		}
		if (ImGui::IsItemHovered())
			SetLastHoveredNode(&parentNode);
	}
	ImGui::PopID();
}