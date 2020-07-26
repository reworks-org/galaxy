///
/// Concepts.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_CONCEPTS_HPP_
#define PROTOSTAR_CONCEPTS_HPP_

#include <concepts>
#include <type_traits>

///
/// Core namespace.
///
namespace pr
{
	///
	/// Only class concept.
	///
	template<typename type>
	concept is_class = std::is_class<type>::value;

	///
	/// Concept to restrict templates to non pointers.
	///
	template<typename type>
	concept no_pointer_or_ref = (!std::is_pointer<type>::value) && (!std::is_reference<type>::value);

	///
	/// Arithmetic concept.
	///
	template<typename type>
	concept is_arithmetic = std::is_arithmetic<type>::value;

	///
	/// Ensures concept is not negative.
	///
	template<typename type>
	concept not_negative = requires(type val)
	{
		{val > 0};
	};

	///
	/// NotNegative + IsArithmetic
	///
	template<typename type>
	concept not_negative_arithmetic = is_arithmetic<type>&& not_negative<type>;

	///
	/// Ensures that a pointer is not null.
	///
	template<typename type>
	concept not_nullptr = requires(type val)
	{
		{val != nullptr};
	};

	///
	/// Ensure a parameter is between 0.0f and 1.0f.
	///
	template<typename type>
	concept between_1_and_0 = requires(type val)
	{
		{val <= 1.0f && val >= 0.0f};
	};

} // namespace pr

#endif