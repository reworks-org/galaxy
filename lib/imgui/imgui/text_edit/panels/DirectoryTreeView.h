#pragma once

#include "../../imgui.h"
#include <string>
#include <vector>
#include <thread>
#include <filesystem>

#include "../Trie.h"

#define FIND_FILES_BUFFER_SIZE 256


struct DirectoryTreeViewNode
{
	std::string fullPath;
	std::string fileName;
	std::vector<DirectoryTreeViewNode> children;
	bool isDirectory;
};

struct DirectoryTreeView
{
	typedef void (*OnNodeFoundCallback)(void);
	typedef void (*OnFileClickCallback)(const std::string& filePath, int directoryTreeViewId);
	typedef void (*OnContextMenuCallback)(const std::string& path, int directoryTreeViewId);
	typedef void (*OnFocusedCallback)(int folderViewId);

	DirectoryTreeView(const std::string& folderPath,
		OnNodeFoundCallback onNodeFoundCallback = nullptr,
		OnFileClickCallback onFileClickCallback = nullptr,
		OnFocusedCallback onFocusedCallback = nullptr,
		std::vector<std::pair<std::string, OnContextMenuCallback>>* fileContextMenuOptions = nullptr,
		std::vector<std::pair<std::string, OnContextMenuCallback>>* folderContextMenuOptions = nullptr,
		int id = -1);
	~DirectoryTreeView();
	bool OnImGui(double deltaTime);
	void RunSearch();
	void ShowFile(const std::string& filePath);
	std::string GetFolderPath(const std::string& filePath);
	
private:
	int id = -1;

	std::string fileToHighlight = "";
	float selectionTimer = -1.0f;

	bool requestingFocus = false;
	bool searching = false;
	char findFilesBuffer[FIND_FILES_BUFFER_SIZE];

	std::string panelName = "Folder view";
	std::string directoryPath;

	DirectoryTreeViewNode directoryTreeRoot;
	OnNodeFoundCallback onNodeFoundCallback = nullptr;
	OnFileClickCallback onFileClickCallback = nullptr;
	OnFocusedCallback onFocusedCallback = nullptr;

	bool isHoveringNodeThisFrame = false;
	const DirectoryTreeViewNode* lastHoveredNode = nullptr;
	std::vector<std::pair<std::string, OnContextMenuCallback>>* fileContextMenuOptions;
	std::vector<std::pair<std::string, OnContextMenuCallback>>* folderContextMenuOptions;

	Trie::Node searchTrie;
	std::vector<std::string> searchResults;
	std::unordered_map<std::string, std::vector<std::string>> fileNameToPath;

	std::thread* folderLoaderThread = nullptr;

	void Refresh();
	void RecursivelyAddDirectoryNodes(DirectoryTreeViewNode& parentNode, std::filesystem::directory_iterator directoryIterator);
	void CreateDirectoryNodeTreeFromPath(DirectoryTreeViewNode& rootNode, const std::filesystem::path& rootPath);
	void SetLastHoveredNode(const DirectoryTreeViewNode* node);
	void RecursivelyDisplayDirectoryNode(const DirectoryTreeViewNode& parentNode);

	static void RefreshThread(DirectoryTreeView* target);
};