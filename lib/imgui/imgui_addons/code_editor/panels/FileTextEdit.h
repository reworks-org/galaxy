#pragma once

#include "../TextEditor.h"

struct FileTextEdit
{
	typedef void (*OnFocusedCallback)(int folderViewId);

	FileTextEdit(const char* filePath = nullptr, int id = -1, int createdFromFolderView = -1, OnFocusedCallback onFocusedCallback = nullptr);
	~FileTextEdit();
	bool OnImGui();
	void SetSelection(int startLine, int startChar, int endLine, int endChar);
	const char* GetAssociatedFile();
	void SetShowDebugPanel(bool value);

private:
	// Commands
	void OnReloadCommand();
	void OnLoadFromCommand();
	void OnSaveCommand();

	int id                    = -1;
	int createdFromFolderView = -1;

	OnFocusedCallback onFocusedCallback = nullptr;

	TextEditor* editor     = nullptr;
	bool showDebugPanel    = false;
	bool hasAssociatedFile = false;
	std::string panelName;
	std::string associatedFile;
	int tabSize         = 4;
	int undoIndexInDisk = 0;

	static std::unordered_map<std::string, const TextEditor::LanguageDefinition*> extensionToLanguageDefinition;
};