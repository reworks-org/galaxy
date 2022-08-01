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
		/// \tparam T Any type that can be a reference.
		///
		template<meta::is_object T>
		using Ref = std::reference_wrapper<T>;

		///
		/// Optional reference.
		///
		/// \tparam T Any type that can be a reference.
		///
		template<typename T>
		using OptionalRef = std::optional<std::reference_wrapper<T>>;
	} // namespace meta
} // namespace galaxy

#endif