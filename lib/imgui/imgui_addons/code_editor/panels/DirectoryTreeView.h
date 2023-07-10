#pragma once

#include <imgui.h>
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
	typedef void (*OnFileClickCallback)(const std::string& filePath, int directoryTreeViewId);
	typedef void (*OnContextMenuCallback)(const std::string& path, int directoryTreeViewId);
	typedef void (*OnFocusedCallback)(int folderViewId);

	DirectoryTreeView(const std::string& folderPath,
		OnFileClickCallback fileClickCallback                                                = nullptr,
		OnFocusedCallback onFocusedCallback                                                  = nullptr,
		std::vector<std::pair<std::string, OnContextMenuCallback>>* fileContextMenuOptions   = nullptr,
		std::vector<std::pair<std::string, OnContextMenuCallback>>* folderContextMenuOptions = nullptr,
		int id                                                                               = -1);
	~DirectoryTreeView();
	bool OnImGui();
	void RunSearch();

private:
	int id = -1;

	bool requestingFocus = false;
	bool searching       = false;
	char findFilesBuffer[FIND_FILES_BUFFER_SIZE];

	std::string panelName = "Folder view";
	std::string directoryPath;

	DirectoryTreeViewNode directoryTreeRoot;
	OnFileClickCallback fileClickCallback = nullptr;
	OnFocusedCallback onFocusedCallback   = nullptr;

	bool isHoveringNodeThisFrame                 = false;
	const DirectoryTreeViewNode* lastHoveredNode = nullptr;
	std::vector<std::pair<std::string, OnContextMenuCallback>>* fileContextMenuOptions;
	std::vector<std::pair<std::string, OnContextMenuCallback>>* folderContextMenuOptions;

	Trie::Node searchTrie;
	std::vector<std::string> searchResults;
	std::unordered_map<std::string, std::vector<std::string>> fileNameToPath;

	void Refresh();
	void RecursivelyAddDirectoryNodes(DirectoryTreeViewNode& parentNode, std::filesystem::directory_iterator directoryIterator);
	DirectoryTreeViewNode CreateDirectoryNodeTreeFromPath(const std::filesystem::path& rootPath);
	void SetLastHoveredNode(const DirectoryTreeViewNode* node);
	void RecursivelyDisplayDirectoryNode(const DirectoryTreeViewNode& parentNode);
};