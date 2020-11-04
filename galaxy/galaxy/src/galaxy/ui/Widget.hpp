///
/// Widget.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_WIDGET_HPP_
#define GALAXY_WIDGET_HPP_

#include "galaxy/ui/Tooltip.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// \brief Interactable UI object.
	///
	/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
	/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
	///
	class Widget
	{
		///
		/// Allows for the GUI to modify internals of Widget we don't want publicly exposed.
		///
		friend class GUI;

	public:
		///
		/// Copy constructor.
		///
		Widget(const Widget&) = delete;

		///
		/// Move constructor.
		///
		Widget(Widget&&);

		///
		/// Copy assignment operator.
		///
		Widget& operator=(const Widget&) = delete;

		///
		/// Move assignment operator.
		///
		Widget& operator=(Widget&&);

		///
		/// Destructor.
		///
		virtual ~Widget();

		///
		/// \brief Update widget logic.
		///
		/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
		///
		virtual void update() = 0;

		///
		/// \brief Render the widget.
		///
		/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
		///
		/// \param camera View camera to use when rendering.
		///
		virtual void render(qs::Camera& camera) = 0;

		///
		/// \brief Get ID of this widget.
		///
		/// 0 is reserved for NULL.
		///
		/// \return const unsigned int ID value.
		///
		[[nodiscard]] const unsigned int id() const;

	protected:
		///
		/// constructor.
		///
		Widget();

		///
		/// Pointer to GUI theme.
		///
		UITheme* m_theme;

		///
		/// Tooltip.
		///
		TooltipPtr m_tooltip;

		///
		/// Bounds of label.
		///
		pr::Rect<float> m_bounds;

	private:
		///
		/// Widget ID.
		///
		unsigned int m_id;
	};

	///
	/// Shorthand.
	///
	using WidgetPtr = std::unique_ptr<galaxy::Widget>;
} // namespace galaxy

#endif