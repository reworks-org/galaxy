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
		EnabledFlag() = default;

		///
		/// Copy constructor.
		///
		EnabledFlag(const EnabledFlag&) = default;

		///
		/// Move constructor.
		///
		EnabledFlag(EnabledFlag&&) = default;

		///
		/// Copy assignment operator.
		///
		EnabledFlag& operator=(const EnabledFlag&) = default;

		///
		/// Move assignment operator.
		///
		EnabledFlag& operator=(EnabledFlag&&) = default;

		///
		/// Default destructor.
		///
		~EnabledFlag() = default;
	};
} // namespace galaxy

#endif