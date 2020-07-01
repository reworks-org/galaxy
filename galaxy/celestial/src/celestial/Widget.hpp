///
/// Widget.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_WIDGET_HPP_
#define CELESTIAL_WIDGET_HPP_

#include <qs/graphics/Camera.hpp>
#include <protostar/async/ProtectedArithmetic.hpp>

#include "celestial/Theme.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// \brief Interactable UI object i.e. a button.
	///
	/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
	/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
	///
	class Widget
	{
		friend class GUI;
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
		/// Argument constructor.
		///
		Widget(celestial::Theme* theme) noexcept;

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
		/// \param camera Camera projection to apply to GUI.
		///
		virtual void render(qs::Camera& camera) noexcept = 0;

	private:
		///
		/// Deleted default constructor.
		///
		Widget() noexcept = delete;

	protected:
		///
		/// Widget ID.
		///
		unsigned int m_id;

		///
		/// Pointer to theme object.
		///
		celestial::Theme* m_theme;
	};

	///
	/// Shorthand.
	///
	using WidgetPtr = std::unique_ptr<celestial::Widget>;
}

#endif