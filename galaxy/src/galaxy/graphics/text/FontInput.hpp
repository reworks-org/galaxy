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
			const char* font_filename;
			msdf_atlas::Charset charset;
			msdf_atlas::GlyphIdentifierType glyph_identifier;
			double font_scale;
		};
	} // namespace graphics
} // namespace galaxy

#endif