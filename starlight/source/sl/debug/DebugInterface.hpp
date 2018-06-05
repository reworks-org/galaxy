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

#ifndef NDEBUG

#include "sl/libs/imgui/imguitexteditor.h"

union ALLEGRO_EVENT;
struct ALLEGRO_DISPLAY;

namespace sl
{
	class BaseState;

	///
	/// Expects scripts to be in a subfolder labelled "scripts".
	///
    class DebugInterface final
    {
    public:
		///
		/// Constructor.
		///
		/// \param display ALLEGRO_DISPLAY object.
		/// \param isDisabled Boolean. Set to true to disable.
		///
		DebugInterface(ALLEGRO_DISPLAY* display, bool isDisabled);

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
		/// Draw IMGUI.
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
		bool m_disabled;
		TextEditor m_editor;
    };
}

#endif

#endif