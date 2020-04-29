///
/// Widget.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_WIDGET_HPP_
#define CELESTIAL_WIDGET_HPP_

#include <protostar/async/ProtectedArithmetic.hpp>

#include "celestial/UITheme.hpp"

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
		///
		/// Allow UI to access protected methods.
		///
		friend class UI;

	public:
		///
		/// Virtual destructor.
		///
		virtual ~Widget() noexcept;

		///
		/// Does the widget contain the point x, y.
		///
		/// \param x x-pos of the point.
		/// \param y y-pos of the point.
		///
		virtual bool contains(const int x, const int y) noexcept final;

		///
		/// Set visibility of widget.
		///
		/// \param isVisible Set to true if widget is visible.
		///
		virtual void setVisibility(const bool isVisible) noexcept final;

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

		///
		/// \brief Update the widget.
		///
		/// Don't forget to check for visibility.
		///
		/// \param dt Delta Time.
		///
		virtual void update(protostar::ProtectedDouble* dt) noexcept = 0;

		///
		/// \brief Render the widget.
		///
		/// This should only contain code on rendering the widget.
		/// And don't forget to check for visibility.
		///
		/// \param renderer Renderer to use when drawing.
		///
		virtual void render(qs::Renderer& renderer) noexcept = 0;

		///
		/// Sets theme for widget to use.
		///
		/// \param theme Theme to apply to widget.
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

	protected:
		///
		/// X, Y, width and height of the widget.
		///
		protostar::Rect<int> m_bounds;

		///
		/// Is the widget currently visible.
		///
		protostar::ProtectedBool m_visible;

		///
		/// UITheme pointer.
		///
		celestial::UITheme* m_theme;

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