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
		/// \brief Update the widget.
		///
		/// Don't forget to check for visibility.
		///
		virtual void update() = 0;

		///
		/// \brief Render the widget.
		///
		/// This should only contain code on rendering the widget.
		/// And don't forget to check for visibility and draw the tooltip.
		///
		virtual void render() = 0;

		///
		/// Set offset of widget relative to panel.
		///
		/// \param x x-pos of panel to calc offset.
		/// \param y y-pos of panel to calc offset.
		///
		virtual void setOffset(const int x, const int y) final;

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
		/// Sets the tooltip for this widget.
		///
		/// \param args Arguments for widget to construct.
		///
		/// \return Returns pointer to newly created widget.
		///
		template<typename... Args>
		Tooltip* setTooltip(Args&&... args);

	protected:
		///
		/// Protected constructor. Only want derived classes to construct this.
		///
		/// \param bounds Dimensions of the widget, relative to the panel.
		///
		explicit Widget(const sl::Rect<int>& bounds) noexcept;

	protected:
		///
		/// Dimensional bounds of the widget.
		///
		sl::Rect<int> m_bounds;

		///
		/// Offset X of widget from panel.
		///
		int m_offsetX;

		///
		/// Offset Y of widget from panel.
		///
		int m_offsetY;

		///
		/// Is the widget currently visible. I.e. being rendered.
		///
		bool m_isVisible;

		///
		/// Toggle for drawing tooltip.
		///
		bool m_drawTooltip;

		///
		/// Tooltip for this widget.
		///
		std::unique_ptr<Tooltip> m_tooltip;
	};

	template<typename... Args>
	inline Tooltip* Widget::setTooltip(Args&&... args)
	{
		// Forward arguments make_unique to construct in place.
		m_tooltip = std::make_unique<Tooltip>(std::forward<Args>(args)...);

		// Then return a pointer to object placed.
		return m_tooltip.get();
	}
}

#endif