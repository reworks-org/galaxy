///
/// EnabledFlag.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ENABLEDFLAG_HPP_
#define GALAXY_ENABLEDFLAG_HPP_

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Flag component used to determine if an entity is "alive" or "dead".
	///
	struct EnabledFlag final
	{
		///
		/// Default constructor.
		///
		EnabledFlag() noexcept = default;

		///
		/// Default destructor.
		///
		~EnabledFlag() noexcept = default;
	};
}

#endif