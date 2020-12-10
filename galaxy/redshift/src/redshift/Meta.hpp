///
/// Meta.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_META_HPP_
#define REDSHIFT_META_HPP_

#include <concepts>

#include "redshift/body/KineticBody.hpp"
#include "redshift/body/StaticBody.hpp"

///
/// Core namespace.
///
namespace rs
{
	///
	/// Must be a physics body.
	///
	template<typename Type>
	concept is_body = (std::is_same<Type, StaticBody>::value && std::is_same<Type, KineticBody>::value);

	///
	/// Require that the object can collide.
	///
	template<typename Type>
	concept is_collidable = requires(Type type)
	{
		1 == 1; // temp
			// is collideable concept that checks for aa/bb iunheritence or circle, etc...
			// void collide(concept a, concept b, func )
			// if constexpr concept tyoer is aabb, do that, else do func
	};
} // namespace rs

#endif