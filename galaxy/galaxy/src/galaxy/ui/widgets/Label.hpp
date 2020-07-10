///
/// Label.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LABEL_HPP_
#define GALAXY_LABEL_HPP_

#include <qs/text/Text.hpp>

#include "galaxy/ui/Widget.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Label for UI objects. Does not support tooltips.
	///
	class Label final : public galaxy::Widget
	{
	public:
		///
		/// Constructor.
		///
		/// \param x x pos.
		/// \param y y pos.
		/// \param text Label text.
		/// \param font Name of the font in the UITheme to use.
		/// \param col Colour of the label text.
		/// \param theme This is set for you by UI::add(). Do not set it.
		///
		Label(const float x, const float y, const std::string& text, const std::string& font, const protostar::Colour& col, galaxy::UITheme* theme) noexcept;

		///
		/// Destructor.
		///
		~Label() noexcept override;

		///
		/// \brief Changes current text.
		///
		/// This is not thread safe. Only call on main thread.
		///
		/// \param text New text to set.
		///
		void updateText(const std::string& text);

	private:
		///
		/// Activate widget.
		///
		void activate() noexcept override;

		///
		/// Deactivate widget.
		///
		void deactivate() noexcept override;

		///
		/// \brief Update the widget.
		///
		/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
		///
		/// \param dt Delta Time.
		///
		void update(protostar::ProtectedDouble* dt) noexcept override;

		///
		/// \brief Perform any GL functions on the main thread in prep for rendering.
		///
		/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
		///
		/// \param shader Use to set any shader uniforms.
		///
		void render(qs::Shader& shader) noexcept override;

	private:
		///
		/// Text to display.
		///
		qs::Text m_text;
	};
}

#endif