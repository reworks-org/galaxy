#include "DirectoryTreeView.h"

#define MAX_SEARCH_RESULTS 50
#define SECONDS_TO_SELECT_ON_SHOW_FILE 2.0f

DirectoryTreeView::DirectoryTreeView(
	const std::string& folderPath,
	OnNodeFoundCallback onNodeFoundCallback,
	OnFileClickCallback onFileClickCallback,
	OnFocusedCallback onFocusedCallback,
	std::vector<std::pair<std::string, OnContextMenuCallback>>* fileContextMenuOptions,
	std::vector<std::pair<std::string, OnContextMenuCallback>>* folderContextMenuOptions,
	int id)
{
	this->id = id;
	panelName = "Folder view##" + std::to_string((int)this);
	directoryPath = folderPath;
	this->onNodeFoundCallback = onNodeFoundCallback;
	this->onFileClickCallback = onFileClickCallback;
	this->onFocusedCallback = onFocusedCallback;
	this->fileContextMenuOptions = fileContextMenuOptions;
	this->folderContextMenuOptions = folderContextMenuOptions;
	findFilesBuffer[0] = '\0';
	folderLoaderThread = new std::thread(RefreshThread, this);
}

DirectoryTreeView::~DirectoryTreeView()
{
	for (auto& item : searchTrie.children)
		Trie::Free(item.second);
}

bool DirectoryTreeView::OnImGui(double deltaTime)
{
	bool windowIsOpen = true;
	if (requestingFocus)
		ImGui::SetNextWindowFocus();
	if (ImGui::Begin(panelName.c_str(), &windowIsOpen, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::MenuItem("Run Search"))
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
			bool pressedEnterOnSearchbarAndThereAreSearchResults = searchTextFieldHasFocus && searchResults.size() > 0 && (ImGui::IsKeyDown(ImGuiKey_Enter) || ImGui::IsKeyDown(ImGuiKey_KeypadEnter)); // to be able to press enter right after typing without using arrow keys

			bool callbackCalled = false;
			for (const std::string& searchResult : searchResults)
			{
				if (callbackCalled)
					break;
				bool fileNameIsUnique = fileNameToPath[searchResult].size() == 1;
				for (const std::string& filePath : fileNameToPath[searchResult])
				{
					if (onFileClickCallback != nullptr)
					{
						if ((fileNameIsUnique ? ImGui::Selectable(searchResult.c_str()) : ImGui::Selectable((searchResult + " (" + filePath + ")").c_str())) ||
							pressedEnterOnSearchbarAndThereAreSearchResults ||
							ImGui::IsItemFocused() && (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)))
						{
							onFileClickCallback(filePath, id);
							ImGui::GetIO().ClearInputKeys();
							searching = false;
							callbackCalled = true;
							break;
						}
					}
				}
			}
			ImGui::Separator();
		}

		isHoveringNodeThisFrame = false;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 2.0f });
		ImGui::SetNextItemOpen(true);
		RecursivelyDisplayDirectoryNode(directoryTreeRoot);
		if (selectionTimer > 0.0f)
		{
			selectionTimer -= deltaTime;
			if (selectionTimer <= 0.0f)
				fileToHighlight = "";
		}
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
				if (lastHoveredNode == &directoryTreeRoot && folderLoaderThread == nullptr && ImGui::Selectable("Refresh folder"))
				{
					folderLoaderThread = new std::thread(RefreshThread, this);
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
	// if user wants to close the window, thread has to be joined
	if (!windowIsOpen && folderLoaderThread != nullptr)
	{
		std::thread* threadToJoin = folderLoaderThread;
		folderLoaderThread = nullptr;
		threadToJoin->join();
	}
	return windowIsOpen;
}

void DirectoryTreeView::RunSearch()
{
	requestingFocus = searching = true;
	findFilesBuffer[0] = '\0';
	searchResults.clear();
}

void DirectoryTreeView::ShowFile(const std::string& filePath)
{
	fileToHighlight = filePath;
	selectionTimer = SECONDS_TO_SELECT_ON_SHOW_FILE;
}

void DirectoryTreeView::Refresh()
{
	for (auto& item : searchTrie.children)
		Trie::Free(item.second);
	searchTrie.isWordEnd = false;
	searchTrie.children.clear();
	fileNameToPath.clear();
	searchResults.clear();
	directoryTreeRoot.children.clear();
	CreateDirectoryNodeTreeFromPath(directoryTreeRoot, directoryPath);
	folderLoaderThread = nullptr;
}

void DirectoryTreeView::RecursivelyAddDirectoryNodes(DirectoryTreeViewNode& parentNode, std::filesystem::directory_iterator directoryIterator)
{
	if (folderLoaderThread == nullptr) return;
	for (const std::filesystem::directory_entry& entry : directoryIterator)
	{
		DirectoryTreeViewNode& childNode = parentNode.children.emplace_back();
		childNode.fullPath = entry.path().string();
		childNode.fileName = entry.path().filename().string();
		if (onNodeFoundCallback != nullptr) onNodeFoundCallback();
		if (childNode.isDirectory = entry.is_directory(); childNode.isDirectory)
			RecursivelyAddDirectoryNodes(childNode, std::filesystem::directory_iterator(entry));
		else
		{
			Trie::Insert(&searchTrie, childNode.fileName);
			fileNameToPath[childNode.fileName].push_back(childNode.fullPath);
		}
	}

	auto moveDirectoriesToFront = [](const DirectoryTreeViewNode& a, const DirectoryTreeViewNode& b) { return (a.isDirectory > b.isDirectory); };
	std::sort(parentNode.children.begin(), parentNode.children.end(), moveDirectoriesToFront);
}

void DirectoryTreeView::CreateDirectoryNodeTreeFromPath(DirectoryTreeViewNode& rootNode, const std::filesystem::path& rootPath)
{
	rootNode.fullPath = rootPath.string();
	rootNode.fileName = rootPath.filename().string();
	if (rootNode.isDirectory = std::filesystem::is_directory(rootPath); rootNode.isDirectory)
		RecursivelyAddDirectoryNodes(rootNode, std::filesystem::directory_iterator(rootPath));
}

void DirectoryTreeView::SetLastHoveredNode(const DirectoryTreeViewNode* node)
{
	lastHoveredNode = node;
	isHoveringNodeThisFrame = true;
}

void DirectoryTreeView::RecursivelyDisplayDirectoryNode(const DirectoryTreeViewNode& parentNode)
{
	bool needToSelectNode = false;
	if (fileToHighlight.length() > 0 && GetFolderPath(fileToHighlight).find(parentNode.fullPath) != std::string::npos) // show file in folder functionality
		ImGui::SetNextItemOpen(true);
	else if (fileToHighlight.compare(parentNode.fullPath) == 0)
		needToSelectNode = true;

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
			ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanFullWidth |
			ImGuiTreeNodeFlags_FramePadding | (needToSelectNode ? ImGuiTreeNodeFlags_Selected : 0x0)))
		{
			if (ImGui::IsItemClicked(0))
			{
				if (onFileClickCallback != nullptr)
					onFileClickCallback(parentNode.fullPath, id);
			}
		}
		if (ImGui::IsItemHovered())
			SetLastHoveredNode(&parentNode);
	}
	ImGui::PopID();
}

void DirectoryTreeView::RefreshThread(DirectoryTreeView* target)
{
	target->Refresh();
}

std::string DirectoryTreeView::GetFolderPath(const std::string& filePath)
{
	int i = filePath.length() - 1;
	for (; i > -1 && filePath[i] != '/' && filePath[i] != '\\'; i--) {};
	return filePath.substr(0, i + 1);
}
