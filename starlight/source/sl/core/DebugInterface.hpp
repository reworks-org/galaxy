///
/// DebugInterface.hpp
/// starlight
///
/// Created by reworks on 20/04/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_DEBUGINTERFACE_HPP_
#define STARLIGHT_DEBUGINTERFACE_HPP_

#ifdef _DEBUG

#include "sl/libs/imgui/imgui_texteditor.h"

union ALLEGRO_EVENT;
struct ALLEGRO_DISPLAY;

namespace sl
{
	class BaseState;

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
		/// \param isDisabled Boolean. Set to true to disable.
		///
		DebugInterface(const std::string& scriptFolderPath, ALLEGRO_DISPLAY* display, bool isDisabled);

		///
		/// Destructor.
		///
		~DebugInterface();

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
		/// Toggles if the debug ui is disabled or not.
		///
		bool m_disabled;

		///
		/// This should be the folder in the VFS where the script files are located. I.e. If all scripts
		/// are in a folder called "scripts/" at root then set this to "scripts/".
		///
		std::string m_scriptFolderPath;

		///
		/// Text editor instance for the script editor gui.
		///
		TextEditor m_editor;
    };
}

#endif

#endif