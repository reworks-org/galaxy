///
/// Box2DIntegration.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICS_BOX2DINTEGRATION_HPP_
#define GALAXY_PHYSICS_BOX2DINTEGRATION_HPP_

#include "galaxy/physics/b2_body.hpp"
#include "galaxy/physics/b2_circle_shape.hpp"
#include "galaxy/physics/b2_edge_shape.hpp"
#include "galaxy/physics/b2_fixture.hpp"
#include "galaxy/physics/b2_polygon_shape.hpp"
#include "galaxy/physics/b2_world.hpp"

namespace galaxy
{
	namespace physics
	{
		///
		/// Scale value for pixel and meter conversions.
		///
		inline const constexpr float SCALE = 100.0f;

		///
		/// \brief Conversion value for OpenGL pixels to Box2D meters.
		///
		/// Pass pixel values multiplied by this to Box2D.
		///
		inline const constexpr float FROM_PIXELS_TO_METERS = 1.0f / SCALE;

		///
		/// \brief Conversion value for Box2D meters to OpenGL pixels.
		///
		/// Pass meter values multiplied by this to OpenGL.
		///
		inline const constexpr float FROM_METERS_TO_PIXELS = SCALE / 1.0f;

		///
		/// Struct holding body configuration data.
		///
		struct BodyConfig final
		{
			b2BodyDef m_def;
			b2FixtureDef m_fixture_def;

			std::shared_ptr<b2Shape> m_shape;
		};
	} // namespace physics
} // namespace galaxy

#endif