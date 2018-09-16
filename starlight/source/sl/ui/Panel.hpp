///
/// Panel.hpp
/// starlight
///
/// Created by reworks on 14/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_PANEL_HPP_
#define STARLIGHT_PANEL_HPP_

#include "sl/ui/Widget.hpp"

namespace sl
{
	///
	/// A UI Panel holds and positions UI widgets on it.
	///
	class Panel
	{
	public:
		///
		/// Constructor.
		///
		Panel();

		///
		/// Destructor.
		///
		~Panel();

		///
		/// Add a new widget to the panel.
		///
		void add(Widget* widget);

	private:
		///
		/// Collection of widgets on the panel.
		///
		std::vector<Widget*> m_widgets;
	};
}

#endif