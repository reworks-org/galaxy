///
/// BodyType.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PHYSICS_BODYTYPE_HPP_
#define GALAXY_PHYSICS_BODYTYPE_HPP_

namespace galaxy
{
	namespace physics
	{
		///
		/// Different types of rigid bodies.
		///
		enum class BodyType : int
		{
			///
			/// Dynamic body (can move).
			///
			DYNAMIC,

			///
			/// Static body (cant move).
			///
			STATIC
		};
	} // namespace physics
} // namespace galaxy

#endif