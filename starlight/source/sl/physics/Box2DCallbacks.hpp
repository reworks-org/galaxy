///
/// Box2DCallbacks.hpp
/// starlight
///
/// Created by reworks on 13/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_BOX2DCALLBACKS_HPP_
#define STARLIGHT_BOX2DCALLBACKS_HPP_

#include "sl/libs/Box2D/Dynamics/b2WorldCallbacks.h"

namespace sl
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
		~CollisionContact() override = default;

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