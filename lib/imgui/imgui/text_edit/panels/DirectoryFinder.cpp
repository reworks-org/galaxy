#include "DirectoryFinder.h"

#include <filesystem>
#include <fstream>

#include "../FontManager.h"
#include "../Utils.h"

DirectoryFinder::DirectoryFinder(const std::string& folderPath,
	OnResultClickCallback onResultClickCallback,
	OnResultFoundCallback onResultFoundCallback,
	OnSearchFinishedCallback onSearchFinishedCallback,
	OnFocusedCallback onFocusedCallback,
	int id, int createdFromFolderView)
{
	this->id = id;
	this->createdFromFolderView = createdFromFolderView;
	this->onResultClickCallback = onResultClickCallback;
	this->onResultFoundCallback = onResultFoundCallback;
	this->onSearchFinishedCallback = onSearchFinishedCallback;
	this->onFocusedCallback = onFocusedCallback;
	panelName = "Folder search##" + std::to_string((int)this);
	strcpy(directoryPath, folderPath.c_str());
	directoryPath[folderPath.length()] = '\0';
}

bool DirectoryFinder::OnImGui()
{
	ImFont* codeFont = FontManager::GetCodeFont(FontManager::GetDefaultUiFontSize());

	bool windowIsOpen = true;
	if (ImGui::Begin(panelName.c_str(), &windowIsOpen, ImGuiWindowFlags_NoSavedSettings))
	{
		if (ImGui::IsWindowFocused() && onFocusedCallback != nullptr)
			onFocusedCallback(this->createdFromFolderView);

		ImGui::InputText("Folder", directoryPath, MAX_PATH_LENGTH);
		ImGui::Checkbox("Regular expression", &regexEnabled);
		ImGui::Checkbox("Case sensitive", &caseSensitiveEnabled);
		ImGui::InputText("To find", toFind, INPUT_BUFFER_SIZE);
		ImGui::InputText("To include", toInclude, INPUT_BUFFER_SIZE);
		ImGui::InputText("To exclude", toExclude, INPUT_BUFFER_SIZE);
		if (finderThread == nullptr)
		{
			if (ImGui::Button("Find"))
				finderThread = new std::thread(&DirectoryFinder::Find, this);
		}
		else
		{
			if (ImGui::Button("Cancel search"))
				finderThread = nullptr;
		}
		ImGui::Separator();
		if (ImGui::InputText("Result filter", resultFilter, INPUT_BUFFER_SIZE) && resultFilter[0] != '\0')
			resultFilterRegex = std::regex(resultFilter);

		{
			std::lock_guard<std::mutex> guard(finderThreadMutex);
			for (int i = 0; i < resultFiles.size(); i++)
			{
				resultsInFile.clear();
				DirectoryFinderSearchResultFile& file = resultFiles[i];
				for (int j = 0; j < file.results.size(); j++)
				{
					DirectoryFinderSearchResult& res = file.results[j];
					std::smatch resultFilterMatch;
					if (resultFilter[0] == '\0' || std::regex_search(res.displayText, resultFilterMatch, resultFilterRegex))
						resultsInFile.push_back(&res);
				}

				if (resultsInFile.size() > 0)
				{
					ImGui::Separator();
					ImGui::TextUnformatted(file.fileName.c_str());
					if (codeFont != nullptr) ImGui::PushFont(codeFont);
					for (DirectoryFinderSearchResult* res : resultsInFile)
						if (ImGui::Selectable(res->displayText.c_str()) && onResultClickCallback != nullptr)
							onResultClickCallback(file.filePath, *res, createdFromFolderView);
					if (codeFont != nullptr) ImGui::PopFont();
				}
			}
		}
	}
	ImGui::End();
	// if user wants to close the window, thread has to be joined so mutex can't be locked
	if (!windowIsOpen && finderThread != nullptr)
	{
		std::thread* threadToJoin = finderThread;
		finderThread = nullptr;
		threadToJoin->join();
	}
	return windowIsOpen;
}

void DirectoryFinder::Find()
{
	// validate
	std::string toFindAsStdString = std::string(toFind);
	std::regex toIncludeAsPattern = std::regex(toInclude);
	std::regex toExcludeAsPattern = std::regex(toExclude);
	std::regex toFindAsPattern;
	if (regexEnabled)
	{
		try { toFindAsPattern = caseSensitiveEnabled ? std::regex(toFind) : std::regex(toFind, std::regex_constants::icase); }
		catch (...) { finderThread = nullptr; return; }
	}

	// start work
	{
		std::lock_guard<std::mutex> guard(finderThreadMutex);
		resultFiles.clear();
	}
	bool foundInFile = false;
	for (std::filesystem::recursive_directory_iterator i(Utils::Utf8ToWstring(directoryPath)), end; i != end; ++i)
	{
		if (finderThread == nullptr)
			break;
		if (is_directory(i->path()))
			continue;

		foundInFile = false;

		std::string fileName = i->path().filename().string();
		std::string filePath = i->path().string();

		std::smatch filePathMatch;
		if (toInclude[0] != '\0' && !std::regex_match(filePath, filePathMatch, toIncludeAsPattern))
			continue;
		if (toExclude[0] != '\0' && std::regex_match(filePath, filePathMatch, toExcludeAsPattern))
			continue;

		std::ifstream fileInput;
		fileInput.open(Utils::Utf8ToWstring(filePath));
		std::string line;
		int curLine = 0;
		while (std::getline(fileInput, line))
		{
			if (finderThread == nullptr)
				break;
			curLine++;
			if (!regexEnabled)
			{
				if (caseSensitiveEnabled)
				{
					int startChar = line.find(toFind, 0);
					if (startChar != std::string::npos)
					{
						std::lock_guard<std::mutex> guard(finderThreadMutex);
						if (!foundInFile)
						{
							resultFiles.push_back({ filePath, fileName, {} });
							foundInFile = true;
						}
						resultFiles.back().results.push_back({ std::to_string(curLine) + ": " + line, curLine, startChar, startChar + (int)toFindAsStdString.length()});
						if (onResultFoundCallback != nullptr) onResultFoundCallback();
					}
				}
				else // caseSensitiveEnabled
				{
					auto it = std::search(
						line.begin(), line.end(),
						toFindAsStdString.begin(), toFindAsStdString.end(),
						[](unsigned char ch1, unsigned char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
					);
					if (it != line.end())
					{
						std::lock_guard<std::mutex> guard(finderThreadMutex);
						int startChar = it - line.begin();
						if (!foundInFile)
						{
							resultFiles.push_back({ filePath, fileName, {} });
							foundInFile = true;
						}
						resultFiles.back().results.push_back({ std::to_string(curLine) + ": " + line, curLine, startChar, startChar + (int)toFindAsStdString.length() });
						if (onResultFoundCallback != nullptr) onResultFoundCallback();
					}
				}
			}
			else // regexEnabled
			{
				std::smatch lineMatch;
				if (std::regex_search(line, lineMatch, toFindAsPattern))
				{
					std::lock_guard<std::mutex> guard(finderThreadMutex);
					if (!foundInFile)
					{
						resultFiles.push_back({ filePath, fileName, {} });
						foundInFile = true;
					}
					resultFiles.back().results.push_back({ std::to_string(curLine) + ": " + line, curLine, (int)lineMatch.position(), (int)(lineMatch.position() + lineMatch.length()) });
					if (onResultFoundCallback != nullptr) onResultFoundCallback();
				}
			}
		}
		fileInput.close();
	}

	finderThread = nullptr;
	if (onSearchFinishedCallback != nullptr) onSearchFinishedCallback();
}