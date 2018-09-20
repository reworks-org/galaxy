///
/// Widget.hpp
/// starlight
///
/// Created by reworks on 15/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_WIDGET_HPP_
#define STARLIGHT_WIDGET_HPP_

#include "sl/math/Rect.hpp"

namespace sl
{
	///
	/// Represents an interactable UI object i.e. a button.
	///
	class Widget
	{
	public:
		///
		/// Destructor.
		///
		virtual ~Widget();

		///
		/// Set visibility.
		///
		void isVisible(bool isVisible);

		///
		/// Update the UI.
		///
		virtual void update() = 0;

		///
		/// \brief Render the Widget.
		///
		/// This should only contain code on rendering the widget. Remember to check for visibility.
		///
		virtual void render() = 0;

	protected:
		///
		/// Protected constructor. Only want derived classes to construct this.
		///
		/// \param bounds Dimensions of the widget, relative to the panel.
		///
		explicit Widget(const sl::Rect<int>& bounds) noexcept;

	public:
		///
		/// Dimensional bounds of the widget.
		///
		sl::Rect<int> m_bounds;

	protected:
		///
		/// Is the panel currently visible. I.e. being rendered.
		///
		bool m_isVisible;
	};
}

#endif