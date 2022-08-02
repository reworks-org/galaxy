///
/// Memory.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_MEMORY_HPP_
#define GALAXY_META_MEMORY_HPP_

#include <memory>
#include <optional>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace meta
	{
		///
		/// \brief Reference wrapper.
		///
		/// Stores a pointer that can only be accessed as a reference.
		///
		/// \tparam Type Any type that can be a reference.
		///
		template<meta::is_object Type>
		using Ref = std::reference_wrapper<Type>;

		///
		/// Optional reference.
		///
		/// \tparam Type Any type that can be a reference.
		///
		template<typename Type>
		using OptionalRef = std::optional<std::reference_wrapper<Type>>;
	} // namespace meta
} // namespace galaxy

#endif