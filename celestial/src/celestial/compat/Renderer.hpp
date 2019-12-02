///
/// Renderer.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef RENDERER_CELESTIAL_HPP_
#define RENDERER_CELESTIAL_HPP_

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
			///
			///
			virtual void drawTexture(const celestial::Texture* texture, const int x, const int y) = 0;

			virtual void drawTexture(const std::string& texture, const int x, const int y) = 0;

			virtual void drawText(const celestial::Font* font, const celestial::Colour* col, const std::string& text, const int x, const int y) = 0;

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