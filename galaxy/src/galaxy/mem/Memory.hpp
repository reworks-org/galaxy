///
/// Memory.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MEM_MEMORY_HPP_
#define GALAXY_MEM_MEMORY_HPP_

#include <optional>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace mem
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
	} // namespace mem
} // namespace galaxy

#endif
