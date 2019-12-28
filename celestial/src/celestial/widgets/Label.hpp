///
/// Label.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_LABEL_HPP_
#define CELESTIAL_LABEL_HPP_

#include "celestial/ui/Widget.hpp"

namespace celestial
{
	///
	/// Label for UI objects. Does not support tooltips.
	///
	class Label final : public Widget
	{
	public:
		///
		/// Text Constructor.
		///
		/// \param x x pos relative to UI panel.
		/// \param y y pos relative to UI panel.
		/// \param text Label text.
		/// \param theme Theme to use with widget.
		///
		Label(const int x, const int y, const std::string& text, UITheme* theme);

		///
		/// Destructor.
		///
		~Label() noexcept;

		///
		/// Update the widget.
		///
		/// \param dt Delta Time.
		///
		void update(const double dt) override;

		///
		/// Render the widget.
		///
		/// Does not render the tooltip.
		///
		void render(celestial::Renderer* renderer) override;

		///
		/// Changes current text.
		///
		/// \param text New text to set.
		///
		void setText(const std::string& text);

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
		void setOffset(const int x, const int y) override;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Label() = delete;

	private:
		///
		/// Text to display.
		///
		std::string m_text;
	};
}

#endif