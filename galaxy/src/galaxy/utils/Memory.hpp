///
/// Memory.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MEM_MEMORY_HPP_
#define GALAXY_MEM_MEMORY_HPP_

#include <optional>
#include <vector>

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
		/// \tparam Object Any type that can be a reference.
		///
		template<meta::is_object Object>
		using ref = std::reference_wrapper<Object>;

		///
		/// Optional reference.
		///
		/// \tparam Object Any type that can be a reference.
		///
		template<meta::is_object Object>
		using opt_ref = std::optional<std::reference_wrapper<Object>>;

		///
		/// \brief Vector reference.
		///
		/// \tparam Type Vector type.
		///
		/// Reference wrapped std::vector.
		///
		template<typename Type>
		using vec_ref = std::reference_wrapper<std::vector<Type>>;
	} // namespace mem
} // namespace galaxy

#endif
