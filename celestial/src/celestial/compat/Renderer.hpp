///
/// Renderer.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_RENDERER_HPP_
#define CELESTIAL_RENDERER_HPP_

#include <string>

#include "celestial/compat/Font.hpp"
#include "celestial/compat/Texture.hpp"

namespace celestial
{
	namespace compat
	{
		///
		/// Interface to be implemented to enable celestial to render.
		///
		class Renderer
		{
		public:
			///
			/// Virtual Destructor.
			///
			virtual ~Renderer() noexcept = default;

			///
			/// Draws a texture to a screen.
			///
			/// \param texture Texture to draw to the screen.
			/// \param x x-position to draw to.
			/// \param y y-position to draw to.
			///
			virtual void drawTexture(const celestial::Texture* texture, const float x, const float y) = 0;

			///
			/// Draws a region of a texture to a screen.
			///
			/// \param texture Texture to draw to the screen.
			/// \param x x-position to draw to.
			/// \param y y-position to draw to.
			/// \param region Region on the texture to draw.
			///
			virtual void drawTextureRegion(const celestial::Texture* texture, const float x, const float y, const celestial::Rect<int>& region) = 0;

			///
			/// Draws text to the screen.
			///
			/// \param font Font to get size and style.
			/// \param col Colour of the text to draw.
			/// \param text Text to draw.
			/// \param x x-position to draw to.
			/// \param y y-position to draw to.
			///
			virtual void drawText(const celestial::Font* font, const celestial::compat::Colour* col, const std::string& text, const float x, const float y) = 0;

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
			virtual void drawMultilineText(const celestial::Font* font, const celestial::compat::Colour* col, const std::string& text, const float x, const float y, const unsigned int maxWidth, const int lineHeight) = 0;

		protected:
			///
			/// Constructor.
			///
			Renderer() noexcept = default;

		private:
			///
			/// Copy constructor.
			///
			Renderer(const Renderer&) = delete;

			///
			/// Move constructor.
			///
			Renderer(Renderer&&) = delete;
		};
	}
}

#endif