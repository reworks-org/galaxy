///
/// WidgetType.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_WIDGETTYPE_HPP_
#define GALAXY_UI_WIDGETTYPE_HPP_

namespace galaxy
{
	namespace ui
	{
		///
		/// The type of a widget.
		///
		enum class WidgetType : int
		{
			BUTTON,
			IMAGE,
			LABEL,
			PROGRESSBAR,
			SLIDER,
			TEXTBOX,
			TEXTINPUT,
			TOGGLEBUTTON
		};
	} // namespace ui
} // namespace galaxy

#endif