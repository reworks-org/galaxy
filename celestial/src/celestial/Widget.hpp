///
/// Widget.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_WIDGET_HPP_
#define CELESTIAL_WIDGET_HPP_

#include "celestial/Tooltip.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Represents an interactable UI object i.e. a button.
	///
	class Widget
	{
	public:
		///
		/// Virtual destructor.
		///
		virtual ~Widget() noexcept;

		///
		/// \brief Update the widget.
		///
		/// Don't forget to check for visibility.
		///
		/// \param dt Delta Time.
		///
		virtual void update(const double dt) noexcept = 0;

		///
		/// \brief Render the widget.
		///
		/// This should only contain code on rendering the widget.
		/// And don't forget to check for visibility and draw the tooltip.
		///
		/// \param renderer Renderer to use when drawing. Needs to be consistent.
		///
		virtual void render(celestial::interface::Renderer* renderer) noexcept = 0;

		///
		/// Does the widget contain the point x, y.
		///
		/// \param x x-pos of the point.
		/// \param y y-pos of the point.
		///
		virtual bool contains(const int x, const int y) noexcept final;

		///
		/// \brief Creates a tooltip for the widget.
		///
		/// Tooltip memory is owned by the widget.
		/// Tooltip theme is set to Widget's theme.
		///
		/// \return Pointer to newly created tooltip.
		///
		virtual Tooltip* createTooltip() noexcept final;

		///
		/// Set visibility of widget.
		///
		/// \param isVisible Set to true if widget is visible.
		///
		virtual void isVisible(const bool isVisible) noexcept final;

		///
		/// Set UITheme.
		///
		/// \param theme Pointer to theme to use.
		///
		virtual void setTheme(celestial::UITheme* theme) noexcept final;

		///
		/// Set bounds.
		///
		/// \param x X pos of widget.
		/// \param y Y pos of widget.
		/// \param w Width of widget.
		/// \param h Height of widget.
		///
		virtual void setBounds(const int x, const int y, const int w, const int h) noexcept final;

		///
		/// Get bounds.
		///
		/// \return Returns a reference to the internal rectangle defining bounds.
		///
		virtual const protostar::Rect<int>& getBounds() const noexcept final;

		///
		/// Get ID of this widget.
		///
		/// \return const unsigned int ID value.
		///
		virtual const unsigned int id() const noexcept final;

	protected:
		///
		/// Default constructor.
		///
		Widget() noexcept;

		///
		/// Copy constructor.
		///
		Widget(const Widget&) noexcept = default;

		///
		/// Move constructor.
		///
		Widget(Widget&&) noexcept = default;

	protected:
		///
		/// X, Y, width and height of the widget.
		///
		protostar::Rect<int> m_bounds;

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

	///
	/// Shorthand.
	///
	using WidgetPtr = std::unique_ptr<celestial::Widget>;
}

#endif