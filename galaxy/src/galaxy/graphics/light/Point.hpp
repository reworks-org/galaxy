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
		/// Point light in a shader.
		///
		class Point : public Light
		{
		public:
			///
			/// Different distances the light can be emitted out to.
			///
			enum class Ranges : short
			{
				DIST_7,
				DIST_13,
				DIST_20,
				DIST_32,
				DIST_50,
				DIST_65,
				DIST_100,
				DIST_160,
				DIST_200,
				DIST_325,
				DIST_600,
				DIST_3250
			};

		public:
			///
			/// Constructor.
			///
			Point() noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~Point() noexcept = default;

			///
			/// Set attunement based on range.
			///
			/// \param dist How far the point light emissions reach.
			///
			void set_attunement(const Ranges dist) noexcept;

			///
			/// Get linear attunement value.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_linear() const noexcept;

			///
			/// Get quadratic attunement value.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float get_quadratic() const noexcept;

		public:
			///
			/// Position of the light.
			///
			glm::vec3 m_pos;

		protected:
			///
			/// Stored for info.
			///
			Ranges m_dist;

			///
			/// Linear attunement value.
			///
			float m_linear;

			///
			/// Quadratic attunement value.
			///
			float m_quadratic;
		};
	} // namespace light
} // namespace galaxy

#endif