///
/// Point.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_POINT_HPP_
#define GALAXY_GRAPHICS_LIGHT_POINT_HPP_

#include "galaxy/graphics/light/Light.hpp"

namespace galaxy
{
	namespace light
	{
		///
		/// Applies a point light to a scene.
		///
		class Point final : public Light
		{
		public:
			///
			/// Constructor.
			///
			Point() noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~Point() noexcept = default;

		public:
			///
			/// Position of the light.
			///
			glm::vec3 m_pos;
		};
	} // namespace light
} // namespace galaxy

#endif