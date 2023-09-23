#include "FileTextEdit.h"

#include <filesystem>
#include <fstream>

#include "../portable-file-dialogs.h"
#include "../Utils.h"

#define FIND_POPUP_TEXT_FIELD_LENGTH 128

std::unordered_map<std::string, TextEditor::LanguageDefinitionId> FileTextEdit::extensionToLanguageDefinition = {
	{".cpp", TextEditor::LanguageDefinitionId::Cpp},
	{".cc", TextEditor::LanguageDefinitionId::Cpp},
	{".hpp", TextEditor::LanguageDefinitionId::Cpp},
	{".h", TextEditor::LanguageDefinitionId::Cpp},
	{".hlsl", TextEditor::LanguageDefinitionId::Hlsl},
	{".glsl", TextEditor::LanguageDefinitionId::Glsl},
	{".py", TextEditor::LanguageDefinitionId::Python},
	{".c", TextEditor::LanguageDefinitionId::C},
	{".sql", TextEditor::LanguageDefinitionId::Sql},
	{".as", TextEditor::LanguageDefinitionId::AngelScript},
	{".lua", TextEditor::LanguageDefinitionId::Lua},
	{".cs", TextEditor::LanguageDefinitionId::Cs},
	{".json", TextEditor::LanguageDefinitionId::Json}};
std::unordered_map<TextEditor::LanguageDefinitionId, const char*> FileTextEdit::languageDefinitionToName = {{TextEditor::LanguageDefinitionId::None, "None"},
	{TextEditor::LanguageDefinitionId::Cpp, "C++"},
	{TextEditor::LanguageDefinitionId::C, "C"},
	{TextEditor::LanguageDefinitionId::Cs, "C#"},
	{TextEditor::LanguageDefinitionId::Python, "Python"},
	{TextEditor::LanguageDefinitionId::Lua, "Lua"},
	{TextEditor::LanguageDefinitionId::Json, "Json"},
	{TextEditor::LanguageDefinitionId::Sql, "SQL"},
	{TextEditor::LanguageDefinitionId::AngelScript, "AngelScript"},
	{TextEditor::LanguageDefinitionId::Glsl, "GLSL"},
	{TextEditor::LanguageDefinitionId::Hlsl, "HLSL"}};
std::unordered_map<TextEditor::PaletteId, const char*> FileTextEdit::colorPaletteToName                  = {{TextEditor::PaletteId::Dark, "Dark"},
					 {TextEditor::PaletteId::Light, "Light"},
					 {TextEditor::PaletteId::Mariana, "Mariana"},
					 {TextEditor::PaletteId::RetroBlue, "Retro blue"}};

FileTextEdit::FileTextEdit(const char* filePath,
	int id,
	int createdFromFolderView,
	OnFocusedCallback onFocusedCallback,
	OnShowInFolderViewCallback onShowInFolderViewCallback)
{
	this->id                         = id;
	this->createdFromFolderView      = createdFromFolderView;
	this->onFocusedCallback          = onFocusedCallback;
	this->onShowInFolderViewCallback = onShowInFolderViewCallback;
	editor                           = new TextEditor();
	if (filePath == nullptr)
		panelName = "untitled##" + std::to_string((int)this);
	else
	{
		hasAssociatedFile = true;
		associatedFile    = std::string(filePath);
		auto pathObject   = std::filesystem::path(filePath);
		panelName         = pathObject.filename().string() + "##" + std::to_string((int)this);
		std::ifstream t(Utils::Utf8ToWstring(filePath));
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		editor->SetText(str);
		auto lang = extensionToLanguageDefinition.find(pathObject.extension().string());
		if (lang != extensionToLanguageDefinition.end())
			editor->SetLanguageDefinition(lang->second);
	}
}

FileTextEdit::~FileTextEdit()
{
	delete editor;
}

bool FileTextEdit::OnImGui()
{
	bool windowIsOpen = true;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
	ImGui::Begin(panelName.c_str(),
		&windowIsOpen,
		ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoSavedSettings | (editor->GetUndoIndex() != undoIndexInDisk ? ImGuiWindowFlags_UnsavedDocument : 0x0));
	ImGui::PopStyleVar();

	if (ImGui::IsWindowFocused() && onFocusedCallback != nullptr)
		onFocusedCallback(this->createdFromFolderView);

	bool isFocused               = ImGui::IsWindowFocused();
	bool requestingGoToLinePopup = false;
	bool requestingFindPopup     = false;
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (hasAssociatedFile && ImGui::MenuItem("Reload", "Ctrl+R"))
				OnReloadCommand();
			if (ImGui::MenuItem("Load from"))
				OnLoadFromCommand();
			if (ImGui::MenuItem("Save", "Ctrl+S"))
				OnSaveCommand();
			if (this->hasAssociatedFile && this->onShowInFolderViewCallback != nullptr && this->createdFromFolderView > -1 &&
				ImGui::MenuItem("Show in folder view"))
				this->onShowInFolderViewCallback(this->associatedFile, this->createdFromFolderView);
			;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = editor->IsReadOnlyEnabled();
			if (ImGui::MenuItem("Read only mode enabled", nullptr, &ro))
				editor->SetReadOnlyEnabled(ro);
			bool ai = editor->IsAutoIndentEnabled();
			if (ImGui::MenuItem("Auto indent on enter enabled", nullptr, &ai))
				editor->SetAutoIndentEnabled(ai);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor->CanUndo()))
				editor->Undo();
			if (ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, !ro && editor->CanRedo()))
				editor->Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl+C", nullptr, editor->AnyCursorHasSelection()))
				editor->Copy();
			if (ImGui::MenuItem("Cut", "Ctrl+X", nullptr, !ro && editor->AnyCursorHasSelection()))
				editor->Cut();
			if (ImGui::MenuItem("Paste", "Ctrl+V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor->Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor->SelectAll();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::SliderInt("Tab size", &tabSize, 1, 8);
			ImGui::SliderFloat("Line spacing", &lineSpacing, 1.0f, 2.0f);
			editor->SetTabSize(tabSize);
			editor->SetLineSpacing(lineSpacing);
			static bool showSpaces = editor->IsShowWhitespacesEnabled();
			if (ImGui::MenuItem("Show spaces", nullptr, &showSpaces))
				editor->SetShowWhitespacesEnabled(!(editor->IsShowWhitespacesEnabled()));
			static bool showLineNumbers = editor->IsShowLineNumbersEnabled();
			if (ImGui::MenuItem("Show line numbers", nullptr, &showLineNumbers))
				editor->SetShowLineNumbersEnabled(!(editor->IsShowLineNumbersEnabled()));
			static bool showShortTabs = editor->IsShortTabsEnabled();
			if (ImGui::MenuItem("Short tabs", nullptr, &showShortTabs))
				editor->SetShortTabsEnabled(!(editor->IsShortTabsEnabled()));
			if (ImGui::BeginMenu("Language"))
			{
				for (int i = (int)TextEditor::LanguageDefinitionId::None; i <= (int)TextEditor::LanguageDefinitionId::Hlsl; i++)
				{
					bool isSelected = i == (int)editor->GetLanguageDefinition();
					if (ImGui::MenuItem(languageDefinitionToName[(TextEditor::LanguageDefinitionId)i], nullptr, &isSelected))
						editor->SetLanguageDefinition((TextEditor::LanguageDefinitionId)i);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Color scheme"))
			{
				for (int i = (int)TextEditor::PaletteId::Dark; i <= (int)TextEditor::PaletteId::RetroBlue; i++)
				{
					bool isSelected = i == (int)editor->GetPalette();
					if (ImGui::MenuItem(colorPaletteToName[(TextEditor::PaletteId)i], nullptr, &isSelected))
						editor->SetPalette((TextEditor::PaletteId)i);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Find"))
		{
			if (ImGui::MenuItem("Go to line", "Ctrl+G"))
				requestingGoToLinePopup = true;
			if (ImGui::MenuItem("Find", "Ctrl+F"))
				requestingFindPopup = true;
			ImGui::EndMenu();
		}

		int line, column;
		editor->GetCursorPosition(line, column);
		ImGui::Text("%6d/%-6d %6d lines | %s | %s",
			line + 1,
			column + 1,
			editor->GetLineCount(),
			editor->IsOverwriteEnabled() ? "Ovr" : "Ins",
			editor->GetLanguageDefinitionName());

		ImGui::EndMenuBar();
	}

	isFocused |= editor->Render("TextEditor", isFocused);
	if (isFocused)
	{
		bool ctrlPressed = ImGui::GetIO().KeyCtrl;
		if (ctrlPressed)
		{
			if (ImGui::IsKeyDown(ImGuiKey_S))
				OnSaveCommand();
			if (ImGui::IsKeyDown(ImGuiKey_R))
				OnReloadCommand();
			if (ImGui::IsKeyDown(ImGuiKey_G))
				requestingGoToLinePopup = true;
			if (ImGui::IsKeyDown(ImGuiKey_F))
				requestingFindPopup = true;
		}
	}

	if (requestingGoToLinePopup)
		ImGui::OpenPopup("go_to_line_popup");
	if (ImGui::BeginPopup("go_to_line_popup"))
	{
		static int targetLine;
		ImGui::SetKeyboardFocusHere();
		ImGui::InputInt("Line", &targetLine);
		if (ImGui::IsKeyDown(ImGuiKey_Enter) || ImGui::IsKeyDown(ImGuiKey_KeypadEnter))
		{
			static int targetLineFixed;
			targetLineFixed = targetLine < 1 ? 0 : targetLine - 1;
			editor->ClearExtraCursors();
			editor->ClearSelections();
			editor->SelectLine(targetLineFixed);
			ImGui::CloseCurrentPopup();
			ImGui::GetIO().ClearInputKeys();
		}
		else if (ImGui::IsKeyDown(ImGuiKey_Escape))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}

	if (requestingFindPopup)
		ImGui::OpenPopup("find_popup");
	if (ImGui::BeginPopup("find_popup"))
	{
		ImGui::Checkbox("Case sensitive", &ctrlfCaseSensitive);
		if (requestingFindPopup)
			ImGui::SetKeyboardFocusHere();
		ImGui::InputText("To find", ctrlfTextToFind, FIND_POPUP_TEXT_FIELD_LENGTH, ImGuiInputTextFlags_AutoSelectAll);
		int toFindTextSize = strlen(ctrlfTextToFind);
		if ((ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) && toFindTextSize > 0)
		{
			editor->ClearExtraCursors();
			editor->SelectNextOccurrenceOf(ctrlfTextToFind, toFindTextSize, ctrlfCaseSensitive);
		}
		if (ImGui::Button("Find all") && toFindTextSize > 0)
			editor->SelectAllOccurrencesOf(ctrlfTextToFind, toFindTextSize, ctrlfCaseSensitive);
		else if (ImGui::IsKeyDown(ImGuiKey_Escape))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::End();

	return windowIsOpen;
}

void FileTextEdit::SetSelection(int startLine, int startChar, int endLine, int endChar)
{
	editor->SetCursorPosition(endLine, endChar);
	editor->SelectRegion(startLine, startChar, endLine, endChar);
}

const char* FileTextEdit::GetAssociatedFile()
{
	if (!hasAssociatedFile)
		return nullptr;
	return associatedFile.c_str();
}

void FileTextEdit::OnFolderViewDeleted(int folderViewId)
{
	if (createdFromFolderView == folderViewId)
		createdFromFolderView = -1;
}

// Commands

void FileTextEdit::OnReloadCommand()
{
	std::ifstream t(Utils::Utf8ToWstring(associatedFile));
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	editor->SetText(str);
	undoIndexInDisk = 0;
}

void FileTextEdit::OnLoadFromCommand()
{
	std::vector<std::string> selection = pfd::open_file("Open file", "", {"Any file", "*"}).result();
	if (selection.size() == 0)
		std::cout << "File not loaded\n";
	else
	{
		std::ifstream t(Utils::Utf8ToWstring(selection[0]));
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		editor->SetText(str);
		auto pathObject = std::filesystem::path(selection[0]);
		auto lang       = extensionToLanguageDefinition.find(pathObject.extension().string());
		if (lang != extensionToLanguageDefinition.end())
			editor->SetLanguageDefinition(extensionToLanguageDefinition[pathObject.extension().string()]);
	}
	undoIndexInDisk = -1; // assume they are loading text from some other file
}

void FileTextEdit::OnSaveCommand()
{
	std::string textToSave  = editor->GetText();
	std::string destination = hasAssociatedFile ? associatedFile : pfd::save_file("Save file", "", {"Any file", "*"}).result();
	if (destination.length() > 0)
	{
		associatedFile    = destination;
		hasAssociatedFile = true;
		panelName         = std::filesystem::path(destination).filename().string() + "##" + std::to_string((int)this);
		std::ofstream outFile(Utils::Utf8ToWstring(destination), std::ios::binary);
		outFile << textToSave;
		outFile.close();
	}
	undoIndexInDisk = editor->GetUndoIndex();
}