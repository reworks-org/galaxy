///
/// Image.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_IMAGE_HPP_
#define CELESTIAL_IMAGE_HPP_

#include "celestial/ui/Widget.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Image for UI objects. Does not support tooltips.
	///
	class Image final : public Widget
	{
	public:
		///
		/// Texture Constructor.
		///
		/// \param x x pos relative to UI panel.
		/// \param y y pos relative to UI panel.
		/// \param texture Name of texture region in theme to use.
		/// \param theme Theme to use with widget.
		///
		Image(const int x, const int y, const std::string& texture, UITheme* theme);

		///
		/// Destructor.
		///
		~Image() noexcept;

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
		Image() = delete;

	private:
		///
		/// Texture being displayed.
		///
		TexturePtr m_image;
	};
}

#endif