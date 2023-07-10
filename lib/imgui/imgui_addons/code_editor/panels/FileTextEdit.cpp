#include "FileTextEdit.h"

#include <filesystem>
#include <fstream>

#include "../portable-file-dialogs.h"
#include "../Utils.h"

#define FIND_POPUP_TEXT_FIELD_LENGTH 128

std::unordered_map<std::string, const TextEditor::LanguageDefinition*> FileTextEdit::extensionToLanguageDefinition = {
	{".cpp", &TextEditor::LanguageDefinition::CPlusPlus()},
	{".cc", &TextEditor::LanguageDefinition::CPlusPlus()},
	{".hpp", &TextEditor::LanguageDefinition::CPlusPlus()},
	{".h", &TextEditor::LanguageDefinition::CPlusPlus()},
	{".hlsl", &TextEditor::LanguageDefinition::HLSL()},
	{".glsl", &TextEditor::LanguageDefinition::GLSL()},
	{".py", &TextEditor::LanguageDefinition::Python()},
	{".c", &TextEditor::LanguageDefinition::C()},
	{".sql", &TextEditor::LanguageDefinition::SQL()},
	{".as", &TextEditor::LanguageDefinition::AngelScript()},
	{".lua", &TextEditor::LanguageDefinition::Lua()},
	{".cs", &TextEditor::LanguageDefinition::CSharp()},
	{".json", &TextEditor::LanguageDefinition::Json()}};

FileTextEdit::FileTextEdit(const char* filePath, int id, int createdFromFolderView, OnFocusedCallback onFocusedCallback)
{
	this->id                    = id;
	this->createdFromFolderView = createdFromFolderView;
	this->onFocusedCallback     = onFocusedCallback;
	editor                      = new TextEditor();
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
			editor->SetLanguageDefinition(*(*lang).second);
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

			if (ImGui::MenuItem("Copy", "Ctrl+C", nullptr, editor->HasSelection()))
				editor->Copy();
			if (ImGui::MenuItem("Cut", "Ctrl+X", nullptr, !ro && editor->HasSelection()))
				editor->Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor->HasSelection()))
				editor->Delete();
			if (ImGui::MenuItem("Paste", "Ctrl+V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor->Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor->SelectAll();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Mariana palette"))
				editor->SetPalette(TextEditor::GetMarianaPalette());
			if (ImGui::MenuItem("Dark palette"))
				editor->SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				editor->SetPalette(TextEditor::GetLightPalette());
			if (ImGui::MenuItem("Retro blue palette"))
				editor->SetPalette(TextEditor::GetRetroBluePalette());
			ImGui::SliderInt("Tab size", &tabSize, 1, 8);
			editor->SetTabSize(tabSize);
			static bool showSpaces = editor->IsShowingWhitespaces();
			if (ImGui::MenuItem("Show spaces", nullptr, &showSpaces))
				editor->SetShowWhitespaces(!(editor->IsShowingWhitespaces()));
			if (ImGui::BeginMenu("Syntax highlighting"))
			{
				if (ImGui::MenuItem("C++"))
					editor->SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
				if (ImGui::MenuItem("C"))
					editor->SetLanguageDefinition(TextEditor::LanguageDefinition::C());
				if (ImGui::MenuItem("C#"))
					editor->SetLanguageDefinition(TextEditor::LanguageDefinition::CSharp());
				if (ImGui::MenuItem("Python"))
					editor->SetLanguageDefinition(TextEditor::LanguageDefinition::Python());
				if (ImGui::MenuItem("Lua"))
					editor->SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
				if (ImGui::MenuItem("Json"))
					editor->SetLanguageDefinition(TextEditor::LanguageDefinition::Json());
				if (ImGui::MenuItem("SQL"))
					editor->SetLanguageDefinition(TextEditor::LanguageDefinition::SQL());
				if (ImGui::MenuItem("AngelScript"))
					editor->SetLanguageDefinition(TextEditor::LanguageDefinition::AngelScript());
				if (ImGui::MenuItem("GLSL"))
					editor->SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
				if (ImGui::MenuItem("HLSL"))
					editor->SetLanguageDefinition(TextEditor::LanguageDefinition::HLSL());

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

		auto cpos = editor->GetCursorPosition();
		ImGui::Text("%6d/%-6d %6d lines | %s | %s",
			cpos.mLine + 1,
			cpos.mColumn + 1,
			editor->GetTotalLines(),
			editor->IsOverwrite() ? "Ovr" : "Ins",
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
			editor->ClearExtraCursors();
			editor->ClearSelections();
			editor->SetCursorPosition({targetLine < 1 ? 0 : targetLine - 1, 0});
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
		static char toFindText[FIND_POPUP_TEXT_FIELD_LENGTH];
		ImGui::SetKeyboardFocusHere();
		ImGui::InputText("To find", toFindText, FIND_POPUP_TEXT_FIELD_LENGTH, ImGuiInputTextFlags_AutoSelectAll);
		if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter))
		{
			editor->ClearExtraCursors();
			editor->SelectNextOccurrenceOf(toFindText, FIND_POPUP_TEXT_FIELD_LENGTH);
		}
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
	editor->SetSelection(startLine, startChar, endLine, endChar);
}

const char* FileTextEdit::GetAssociatedFile()
{
	if (!hasAssociatedFile)
		return nullptr;
	return associatedFile.c_str();
}

void FileTextEdit::SetShowDebugPanel(bool value)
{
	showDebugPanel = value;
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
			editor->SetLanguageDefinition(*extensionToLanguageDefinition[pathObject.extension().string()]);
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
		std::ofstream outFile;
		outFile.open(Utils::Utf8ToWstring(destination));
		outFile << textToSave;
		outFile.close();
	}
	undoIndexInDisk = editor->GetUndoIndex();
}