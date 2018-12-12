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

#include <allegro5/events.h>

#include "sl/math/Rect.hpp"
#include "sl/ui/Tooltip.hpp"

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
		virtual ~Widget() noexcept;

		///
		/// Update the widget.
		///
		virtual void update() = 0;

		///
		/// \brief Render the widget.
		///
		/// This should only contain code on rendering the widget.
		/// And don't forget to check for and draw the tooltip.
		///
		virtual void render() = 0;

		///
		/// Set visibility of widget.
		///
		/// \param isVisible Set to true if widget is visible.
		///
		virtual void setVisibility(const bool isVisible) final;

		///
		/// Is the widget visible?
		///
		/// \return True if visible.
		///
		virtual const bool isVisible() const final;

		///
		/// Sets the tooltip for this widget. Calls std::move() on the passed object.
		///
		/// \param tooltip std::unique_ptr to tooltip. Ideally constructed via std::make_unique.
		///
		virtual void setTooltip(std::unique_ptr<Tooltip> tooltip) final;

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
		/// Is the widget currently visible. I.e. being rendered.
		///
		bool m_isVisible;

		///
		/// Tooltip for this widget.
		///
		std::unique_ptr<Tooltip> m_tooltip;
	};
}

#endif