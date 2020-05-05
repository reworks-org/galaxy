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
	/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
	/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
	///
	class Widget
	{
		///
		/// So UI can directly access stuff that should not be publicly exposed.
		///
		friend class UI;
	public:
		///
		/// Virtual destructor.
		///
		virtual ~Widget() noexcept = default;

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
		/// Activate widget.
		///
		virtual void activate() noexcept = 0;

		///
		/// Deactivate widget.
		///
		virtual void deactivate() noexcept = 0;

		///
		/// \brief Update the widget.
		///
		/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
		///
		/// \param dt Delta Time.
		///
		virtual void update(protostar::ProtectedDouble* dt) noexcept = 0;

		///
		/// \brief Perform any GL functions on the main thread in prep for rendering.
		///
		/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
		///
		/// \param shader Use to set any shader uniforms.
		///
		virtual void render(qs::Shader& shader) noexcept = 0;

	protected:
		///
		/// Widget ID.
		///
		unsigned int m_id;

		///
		/// UITheme pointer.
		///
		celestial::UITheme* m_theme;
	};

	///
	/// Shorthand.
	///
	using WidgetPtr = std::unique_ptr<celestial::Widget>;
}

#endif