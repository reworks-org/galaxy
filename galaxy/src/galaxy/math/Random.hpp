///
/// Random.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_RANDOM_HPP_
#define GALAXY_MATH_RANDOM_HPP_

#include <random>

#include <Raylib.hpp>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace math
	{
		///
		/// Source: http://stackoverflow.com/a/32907541.
		///
		template<typename Type>
		using conditional_distribution = std::conditional_t<
			std::is_integral<Type>::value,
			std::uniform_int_distribution<Type>,
			std::conditional_t<std::is_floating_point<Type>::value, std::uniform_real_distribution<Type>, void>>;

		///
		/// Generate a random number of type T.
		///
		/// \tparam T The type of number to return. Must be arithmetic.
		///
		/// \param min Minimum number inclusive.
		/// \param max Maximum number inclusive.
		///
		/// \return Returns number of the same type as inputs.
		///
		template<meta::is_arithmetic T>
		[[nodiscard]]
		inline T random(const T min, const T max) noexcept
		{
			thread_local std::random_device rd;
			thread_local std::mt19937_64    mt {rd()};

			conditional_distribution<T> dist {min, max};
			return dist(mt);
		}

		///
		/// Generate a random vec2.
		///
		/// \param min Minimum vec2 inclusive.
		/// \param max Maximum vec2 inclusive.
		///
		/// \return Pseudo-randomized vec2.
		///
		[[nodiscard]]
		ray::Vector2 random_vec2(const ray::Vector2& min, const ray::Vector2& max) noexcept;

		///
		/// Generate a random vec3.
		///
		/// \param min Minimum vec3 inclusive.
		/// \param max Maximum vec3 inclusive.
		///
		/// \return Pseudo-randomized vec3.
		///
		[[nodiscard]]
		ray::Vector3 random_vec3(const ray::Vector3& min, const ray::Vector3& max) noexcept;

		///
		/// Generate a random vec4.
		///
		/// \param min Minimum vec4 inclusive.
		/// \param max Maximum vec4 inclusive.
		///
		/// \return Pseudo-randomized vec4.
		///
		[[nodiscard]]
		ray::Vector4 random_vec4(const ray::Vector4& min, const ray::Vector4& max) noexcept;
	} // namespace math
} // namespace galaxy

#endif
