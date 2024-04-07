#pragma once

#include "../../imgui.h"
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <regex>

#define INPUT_BUFFER_SIZE 256
#define MAX_PATH_LENGTH 512

struct DirectoryFinderSearchResult
{
	std::string displayText;
	int lineNumber;
	int startCharIndex;
	int endCharIndex;
};

struct DirectoryFinderSearchResultFile
{
	std::string filePath;
	std::string fileName;
	std::vector<DirectoryFinderSearchResult> results;
};

struct DirectoryFinder
{
	typedef void (*OnResultClickCallback)(const std::string& filePath, const DirectoryFinderSearchResult& result, int originFolderView);
	typedef void (*OnResultFoundCallback)();
	typedef void (*OnSearchFinishedCallback)();
	typedef void (*OnFocusedCallback)(int folderViewId);

	DirectoryFinder(const std::string& folderPath,
		OnResultClickCallback onResultClickCallback = nullptr,
		OnResultFoundCallback onResultFoundCallback = nullptr,
		OnSearchFinishedCallback onSearchFinishedCallback = nullptr,
		OnFocusedCallback onFocusedCallback = nullptr,
		int id = -1, int createdFromFolderView = -1);
	bool OnImGui();

private:
	int id = -1;
	int createdFromFolderView = -1;

	std::string panelName = "Folder search";
	char directoryPath[MAX_PATH_LENGTH] = "\0";
	bool regexEnabled = true;
	bool caseSensitiveEnabled = true;
	char toFind[INPUT_BUFFER_SIZE] = "\0";
	char toInclude[INPUT_BUFFER_SIZE] = "\0";
	char toExclude[INPUT_BUFFER_SIZE] = "\0";
	char resultFilter[INPUT_BUFFER_SIZE] = "\0";
	std::regex resultFilterRegex;

	std::vector<DirectoryFinderSearchResultFile> resultFiles;
	std::vector<DirectoryFinderSearchResult*> resultsInFile;

	OnResultClickCallback onResultClickCallback = nullptr;
	OnResultFoundCallback onResultFoundCallback = nullptr;
	OnSearchFinishedCallback onSearchFinishedCallback = nullptr;
	OnFocusedCallback onFocusedCallback = nullptr;
	std::thread* finderThread = nullptr;
	std::mutex finderThreadMutex;

	void Find();
};