///
/// FontInput.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_FONTINPUT_HPP_
#define GALAXY_GRAPHICS_FONTINPUT_HPP_

#include <msdf-atlas-gen/msdf-atlas-gen.h>

namespace galaxy
{
	namespace graphics
	{
		///
		/// MSDF Font Input settings.
		///
		struct FontInput final
		{
			///
			/// Font filename.
			///
			const char* font_filename;

			///
			/// MSDF charset.
			///
			msdf_atlas::Charset charset;

			///
			/// MSDF Glyph Id.
			///
			msdf_atlas::GlyphIdentifierType glyph_identifier;

			///
			/// Font size.
			///
			double font_scale;
		};
	} // namespace graphics
} // namespace galaxy

#endif