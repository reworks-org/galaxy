///
/// Image.hpp
/// starlight
///
/// Created by reworks on 97/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_IMAGE_HPP_
#define STARLIGHT_IMAGE_HPP_

#include "sl/ui/Widget.hpp"

namespace sl
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
		/// \param texture Texture in theme to use.
		/// \param theme Theme to use with widget.
		///
		Image(const int x, const int y, const std::string& texture, UITheme* theme);

		///
		/// Lua Constructor.
		///
		/// \param table sol::table to create widget from.
		/// \param theme Theme to use with widget.
		///
		Image(const sol::table& table, UITheme* theme);

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
		void render() override;

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
		ALLEGRO_BITMAP* m_image;
	};
}

#endif