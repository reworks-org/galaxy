///
/// AttributeBindings.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_ATTRIBUTEBINDINGS_HPP_
#define GALAXY_GRAPHICS_ATTRIBUTEBINDINGS_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Stores the location of shader attribute binding point for the VAO.
		///
		enum class AttributeBinding : unsigned int
		{
			///
			/// Position data.
			///
			POSITION_POINT = 0,

			///
			/// Texel data.
			///
			TEXEL_POINT = 1,

			///
			/// Colour data.
			///
			COLOUR_POINT = 2,

			///
			/// Instance offset data.
			///
			OFFSET_POINT = 3
		};
	} // namespace graphics
} // namespace galaxy

#endif