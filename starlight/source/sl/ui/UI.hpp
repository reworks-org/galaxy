///
/// UI.hpp
/// starlight
///
/// Created by reworks on 14/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_UI_HPP_
#define STARLIGHT_UI_HPP_

#include <vector>

#include "sl/ui/Panel.hpp"

namespace sl
{
	///
	/// Manages UI for a dynamic array of Panels.
	///
	class UI
	{
	public:
		///
		/// Constructor.
		///
		UI();

		///
		/// Destructor.
		///
		~UI();

		///
		/// Add a Panel to the UI.
		///
		void addPanel(Panel* panel);

		///
		/// Process UI events.
		///
		void event();

		///
		/// Update UI.
		///
		void update();

		///
		/// Render UI to screen.
		///
		void render();

	private:
		///
		/// Data structure housing Panels.
		///
		std::vector<Panel*> m_panels;
	};
}

#endif