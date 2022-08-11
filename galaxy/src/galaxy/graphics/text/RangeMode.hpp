///
/// RangeMode.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RANGEMODE_HPP_
#define GALAXY_GRAPHICS_RANGEMODE_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Specify range configuration for MSDF.
		///
		enum class RangeMode
		{
			///
			/// Range specified in EMs.
			///
			RANGE_EM,

			///
			/// Range specified in output pixels.
			///
			RANGE_PIXEL,
		};
	} // namespace graphics
} // namespace galaxy

#endif