///
/// DebugInterface.hpp
/// galaxy
///
/// Created by reworks on 20/04/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_DEBUGINTERFACE_HPP_
#define GALAXY_DEBUGINTERFACE_HPP_

#ifdef _DEBUG

#include "galaxy/libs/imgui/imgui_texteditor.h"

union ALLEGRO_EVENT;
struct ALLEGRO_DISPLAY;

namespace galaxy
{
	///
	/// Manages the ImGui debug gui that assists in gamedev.
	/// Allows for the creation of some really cool tools.
	///
    class DebugInterface final
    {
    public:
		///
		/// Constructor.
		///
		/// \param scriptFolderPath This should be the folder in the VFS where the script files are located.
		/// \param display ALLEGRO_DISPLAY object.
		///
		DebugInterface(const std::string& scriptFolderPath, ALLEGRO_DISPLAY* display);

		///
		/// Destructor.
		///
		~DebugInterface() noexcept;

		///
		/// Process imgui events.
		///
		/// \param event ALLEGRO_EVENT object.
		///
		void event(ALLEGRO_EVENT* event);

		///
		/// Begin new imgui frame.
		///
		void newFrame();

		///
		/// Draw interface.
		///
		void render();

		///
		/// \brief Display menu on screen.
		///
		/// Call between update and render. Calls the functions that make up the main debug menu. ImGui functions mainly.
		///
		/// \param restart Pointer to boolean controlling if program should restart on exit.
		///
		void displayMenu(bool* restart);

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		DebugInterface() = delete;

	private:
		///
		/// This should be the folder in the VFS where the script files are located. I.e. If all scripts
		/// are in a folder called "scripts/" at root then set this to "scripts/".
		///
		std::string m_scriptFolderPath;

		///
		/// Text editor instance for the script editor gui.
		///
		TextEditor m_editor;

		// Variables that are used internally and are required to use the gui.
		bool m_showLuaConsole = false;
		bool m_showScriptEditor = false;
		std::string m_buff = "";
		std::string m_stateBuff = "";

		int m_index = 0;
		bool m_showFilesToLoad = false;
		std::string m_currentScript = "";
		std::vector<std::string> m_files;
    };
}

#endif

#endif