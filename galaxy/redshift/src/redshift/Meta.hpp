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
		(std::is_base_of_v<Collidable, Type> && std::is_convertible_v<const volatile Type*, const volatile Collidable*>);
	};
} // namespace rs

#endif