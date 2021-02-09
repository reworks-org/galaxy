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
#include "galaxy/physics/b2_contact.hpp"
#include "galaxy/physics/b2_edge_shape.hpp"
#include "galaxy/physics/b2_fixture.hpp"
#include "galaxy/physics/b2_polygon_shape.hpp"
#include "galaxy/physics/b2_world.hpp"

namespace galaxy
{
	namespace events
	{
		class Dispatcher;
	} // namespace events

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

		///
		/// Contact listener override for galaxy.
		///
		class GalaxyContactListener final : public b2ContactListener
		{
		public:
			///
			/// Argument constructor.
			///
			/// \param dispatcher Pointer to a dispatcher to use for collision callbacks.
			///
			GalaxyContactListener(events::Dispatcher* dispatcher);

			///
			/// Destructor.
			///
			virtual ~GalaxyContactListener() noexcept = default;

			///
			/// Called when two fixtures begin to touch.
			///
			void BeginContact(b2Contact* contact) override;

			///
			/// Called when two fixtures cease to touch.
			///
			void EndContact(b2Contact* contact) override;

			///
			/// This is called after a contact is updated. This allows you to inspect a
			/// contact before it goes to the solver. If you are careful, you can modify the
			/// contact manifold (e.g. disable contact).
			/// A copy of the old manifold is provided so that you can detect changes.
			/// Note: this is called only for awake bodies.
			/// Note: this is called even when the number of contact points is zero.
			/// Note: this is not called for sensors.
			/// Note: if you set the number of contact points to zero, you will not
			/// get an EndContact callback. However, you may get a BeginContact callback
			/// the next step.
			///
			void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

			///
			/// This lets you inspect a contact after the solver is finished. This is useful
			/// for inspecting impulses.
			/// Note: the contact manifold does not include time of impact impulses, which can be
			/// arbitrarily large if the sub-step is small. Hence the impulse is provided explicitly
			/// in a separate data structure.
			/// Note: this is only called for contacts that are touching, solid, and awake.
			///
			void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

		private:
			///
			/// Constructor.
			///
			GalaxyContactListener() = delete;

		private:
			///
			/// Event dispatcher reference.
			///
			events::Dispatcher* m_dispatcher;
		};
	} // namespace physics
} // namespace galaxy

#endif