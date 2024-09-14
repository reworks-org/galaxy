///
/// Memory.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UTILS_MEMORY_HPP_
#define GALAXY_UTILS_MEMORY_HPP_

#include "galaxy/utils/Concepts.hpp"

namespace galaxy
{
	namespace utils
	{
		///
		/// \brief Reference wrapper.
		///
		/// Stores a pointer that can only be accessed as a reference.
		///
		/// \tparam Type Any type that can be a reference.
		///
		template<is_object Type>
		using ref = std::reference_wrapper<Type>;

		///
		/// Optional reference.
		///
		/// \tparam Type Any type that can be a reference.
		///
		template<typename Type>
		using optional_ref = std::optional<std::reference_wrapper<Type>>;
	} // namespace utils
} // namespace galaxy

#endif
