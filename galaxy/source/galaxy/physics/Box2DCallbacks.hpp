///
/// Box2DCallbacks.hpp
/// galaxy
///
/// Created by reworks on 13/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_BOX2DCALLBACKS_HPP_
#define GALAXY_BOX2DCALLBACKS_HPP_

#include "galaxy/libs/Box2D/Dynamics/b2WorldCallbacks.h"

namespace galaxy
{
	///
	/// This is the class that handles Box2D collisions for this library.
	///
	class CollisionContact final : public b2ContactListener
	{
	public:
		///
		/// Constructor.
		///
		CollisionContact() = default;

		///
		/// Destructor.
		///
		~CollisionContact() noexcept override = default;

		///
		/// \brief Override the box2d contact functions on collision start.
		///
		/// Refer to Box2D docs on collision.
		///
		void BeginContact(b2Contact* contact) override;

		///
		/// \brief Override the box2d contact functions on collision end.
		///
		/// Refer to Box2D docs on collision.
		///
		void EndContact(b2Contact* contact) override;
	};
}

#endif