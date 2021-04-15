///
/// Spot.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_SPOT_HPP_
#define GALAXY_GRAPHICS_LIGHT_SPOT_HPP_

#include "galaxy/graphics/light/Light.hpp"

namespace galaxy
{
	namespace light
	{
		///
		/// Spot light in a shader.
		///
		class Spot final : public Light
		{
		public:
			///
			/// Constructor.
			///
			Spot() noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~Spot() noexcept = default;

		public:
			///
			/// Position of the light.
			///
			glm::vec3 m_pos;

			///
			/// Direction of the light.
			///
			glm::vec3 m_dir;

			///
			/// Inner cutoff angle.
			///
			float m_inner_cutoff;

			///
			/// Outer cutoff angle.
			///
			float m_outer_cutoff;
		};
	} // namespace light
} // namespace galaxy

#endif