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
	template<typename Type>
	concept IsClass = std::is_class<Type>::value;

	///
	/// Concept to restrict templates to non pointers.
	///
	template<typename Type>
	concept NoPointerOrRef = (!std::is_pointer<Type>::value) && (!std::is_reference<Type>::value);

	///
	/// Arithmetic concept.
	///
	template<typename Type>
	concept IsArithmetic = std::is_arithmetic<Type>::value;

	///
	/// Ensures concept is not negative.
	///
	template<typename Type>
	concept NotNegative = requires(Type a)
	{
		{a > 0};
	};

	///
	/// NotNegative + IsArithmetic
	///
	template<typename Type>
	concept PositiveArithmetic = IsArithmetic<Type> && NotNegative<Type>;

} // namespace pr

#endif