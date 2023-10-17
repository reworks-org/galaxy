///
/// Concepts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_CONCEPTS_HPP_
#define GALAXY_META_CONCEPTS_HPP_

#include <concepts>
#include <type_traits>

#include <robin_hood.h>

namespace galaxy
{
	namespace meta
	{
		///
		/// \brief Only class concept.
		///
		/// Type must be a class or struct.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept is_class = std::is_class<Type>::value;

		///
		/// \brief Concept to restrict templates to not pointers and not references.
		///
		/// Type must not be a pointer or reference.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept not_memory = !std::is_pointer<Type>::value && !std::is_reference<Type>::value && std::is_object<Type>::value;

		///
		/// Arithmetic concept.
		///
		/// Type must be arithmetic. See: std::is_arithmetic.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept is_arithmetic = std::is_arithmetic<Type>::value;

		///
		/// \brief Ensures a type is arithmetic or a std::string.
		///
		/// Must be an arithmetic type or a std::string. See: std::is_arithmetic.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept standard_type = is_arithmetic<Type> || std::is_same<std::string, Type>::value;

		///
		/// \brief Tests for a type being a class, union, array, scalar or integral constant.
		///
		/// "Object" refers to any possibly cv-qualified type other than function, reference, or void types.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept is_object = std::is_object<Type>::value;

		///
		/// Makes sure a value is a valid value for a std::bitset.
		///
		/// Must be an unsigned short between 0 and 7. i.e. 8 values.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept is_bitset_flag = requires (Type type) { Type::value >= 0 && Type::value <= 7 && std::is_same<decltype(Type::value), unsigned short>::value; };

		///
		/// \brief Makes sure a type is a valid component.
		///
		/// I.e. Move assignable/constructible and a class with a json constructor.
		///
		/// \tparam Type Type to test.
		///
		template<typename Type>
		concept valid_component = std::is_move_assignable<Type>::value && std::is_move_constructible<Type>::value && std::is_class<Type>::value;

		///
		/// Concept to enforce a loader that requires an opengl build() function.
		///
		/// \tparam Loader Loader class to test.
		/// \tparam Resource Resource to build in OpenGL.
		///
		template<typename Loader, typename Resource>
		concept loader_build_check =
			requires (Loader loader, robin_hood::unordered_node_map<std::uint64_t, std::shared_ptr<Resource>> resource) { loader.build(resource); };

		///
		/// Ensures that the provided loader has an operator() that returns a shared pointer.
		///
		/// \tparam Loader The loader you want to test.
		/// \tparam Resource The resource being loaded.
		/// \tparam needs_gl Does the loader need a opengl builder function.
		///
		template<typename Loader, typename Resource, bool needs_gl>
		concept is_loader = requires (Loader loader) {
			loader.operator();
			requires !needs_gl || (needs_gl && loader_build_check<Loader, Resource>);
		};
	} // namespace meta
} // namespace galaxy

#endif
