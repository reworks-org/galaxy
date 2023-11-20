///
/// ParticleSpread.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_PARTICLESPREAD_HPP_
#define GALAXY_GRAPHICS_PARTICLESPREAD_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Controls how particles are placed when being randomly generated.
		///
		enum class ParticleSpread
		{
			///
			/// Confine to a spherical area.
			///
			SPREAD_TYPE_SPHERE,

			///
			/// Confine to a rectangular area.
			///
			SPREAD_TYPE_RECTANGLE
		};
	} // namespace graphics
} // namespace galaxy

#endif
