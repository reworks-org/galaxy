///
/// Widget.hpp
/// galaxy
///
/// Created by reworks on 15/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_WIDGET_HPP_
#define GALAXY_WIDGET_HPP_

#include <allegro5/events.h>

#include "galaxy/ui/UITheme.hpp"
#include "galaxy/ui/Tooltip.hpp"

namespace galaxy
{
	///
	/// Represents an interactable UI object i.e. a button.
	///
	class Widget
	{
		friend class Panel;
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
		/// \param dt Delta Time.
		///
		virtual void update(const double dt) = 0;

		///
		/// \brief Render the widget.
		///
		/// This should only contain code on rendering the widget.
		/// And don't forget to check for visibility and draw the tooltip.
		///
		virtual void render() = 0;

		///
		/// Does the widget contain the point x, y.
		///
		/// \param x x-pos of the point.
		/// \param y y-pos of the point.
		///
		virtual bool contains(const int x, const int y) final;

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
		/// Get ID of this widget.
		///
		/// \return const unsigned int ID value.
		///
		virtual const unsigned int id() const final;

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
		/// \param theme Theme to be used by this widget.
		///
		explicit Widget(const galaxy::Rect<int>& bounds, UITheme* theme);

		///
		/// \brief Set the offset of the widget from the panel. Called for you in the Panel::add widget function.
		///
		/// It should look like this:
		/// m_bounds.m_x += x;
		/// m_bounds.m_y += y;
		///
		/// \param x x-pos of the panel.
		/// \param y y-pos of the panel.
		///
		virtual void setOffset(const int x, const int y) = 0;

	protected:
		///
		/// X, Y, width and height of the widget.
		///
		galaxy::Rect<int> m_bounds;

		///
		/// Pointer to the theme used by the widget.
		///
		UITheme* m_theme;

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

	private:
		///
		/// Widget ID.
		///
		unsigned int m_id;
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