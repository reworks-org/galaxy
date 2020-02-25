///
/// Renderer.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_RENDERER_HPP_
#define CELESTIAL_RENDERER_HPP_

#include <string>

#include <protostar/shapes/Rect.hpp>
#include <protostar/graphics/Colour.hpp>

#include "celestial/interface/Font.hpp"
#include "celestial/interface/Texture.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Identifies this class as an interface.
	///
	namespace interface
	{
		///
		/// Interface to be implemented to enable celestial to render.
		///
		class Renderer
		{
		public:
			///
			/// Default virtual destructor.
			///
			virtual ~Renderer() noexcept = default;

			///
			/// Draws a texture to a screen.
			///
			/// \param texture Texture to draw to the screen.
			/// \param x x-position to draw to.
			/// \param y y-position to draw to.
			///
			virtual void drawTexture(const celestial::interface::Texture* texture, const double x, const double y) noexcept = 0;

			///
			/// Draws a region of a texture to a screen.
			///
			/// \param texture Texture to draw to the screen.
			/// \param x x-position to draw to.
			/// \param y y-position to draw to.
			/// \param region Region on the texture to draw.
			///
			virtual void drawTextureRegion(const celestial::interface::Texture* texture, const double x, const double y, const protostar::Rect<int>& region) noexcept = 0;

			///
			/// Draws text to the screen.
			///
			/// \param font Font to get size and style.
			/// \param col Colour of the text to draw.
			/// \param text Text to draw.
			/// \param x x-position to draw to.
			/// \param y y-position to draw to.
			///
			virtual void drawText(const celestial::interface::Font* font, const protostar::Colour& col, const std::string& text, const double x, const double y) noexcept = 0;

			///
			/// Draws multiline text to the screen.
			///
			/// \param font Font to get size and style.
			/// \param col Colour of the text to draw.
			/// \param text Text to draw.
			/// \param x x-position to draw to.
			/// \param y y-position to draw to.
			/// \param maxWidth Maximum width of each line of text.
			/// param lineHeight Line height of the font.
			///
			virtual void drawMultilineText(const celestial::interface::Font* font, const protostar::Colour& col, const std::string& text, const double x, const double y, const unsigned int maxWidth, const int lineHeight) noexcept = 0;

		protected:
			///
			/// Constructor.
			///
			Renderer() noexcept = default;

			///
			/// Copy constructor.
			///
			Renderer(const Renderer&) noexcept = default;

			///
			/// Move constructor.
			///
			Renderer(Renderer&&) noexcept = default;
		};
	}
}

#endif