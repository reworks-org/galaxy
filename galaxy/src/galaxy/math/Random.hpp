///
/// Random.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_RANDOM_HPP_
#define GALAXY_MATH_RANDOM_HPP_

#include <random>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

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
		/// \brief Generates random numbers.
		///
		/// Different than the function in that it doesnt create a new device each call. Essentially faster when needing lots of random numbers.
		///
		class Random final
		{
		public:
			///
			/// Constructor.
			///
			Random() noexcept;

			///
			/// Destructor.
			///
			~Random() noexcept;

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
			T gen(const T min, const T max) noexcept;

			///
			/// Generate a random vec2.
			///
			/// \tparam T The type of number to return. Must be arithmetic.
			///
			/// \param min Minimum vec2 inclusive.
			/// \param max Maximum vec2 inclusive.
			///
			/// \return Pseudo-randomized vec2.
			///

			template<meta::is_arithmetic T>
			[[nodiscard]]
			sf::Vector2<T> gen_vec2(const sf::Vector2<T>& min, const sf::Vector2<T>& max) noexcept;

			///
			/// Generate a random vec3.
			///
			/// \tparam T The type of number to return. Must be arithmetic.
			///
			/// \param min Minimum vec3 inclusive.
			/// \param max Maximum vec3 inclusive.
			///
			/// \return Pseudo-randomized vec3.
			///
			template<meta::is_arithmetic T>
			[[nodiscard]]
			sf::Vector3<T> gen_vec3(const sf::Vector3<T>& min, const sf::Vector3<T>& max) noexcept;

		private:
			///
			/// Randomizer device.
			///
			std::random_device m_rd;

			///
			/// Pseudo-random algorithm.
			///
			std::mt19937_64 m_mt;
		};

		template<meta::is_arithmetic T>
		T Random::gen(const T min, const T max) noexcept
		{
			conditional_distribution<T> dist {min, max};
			return dist(m_mt);
		}

		template<meta::is_arithmetic T>
		sf::Vector2<T> Random::gen_vec2(const sf::Vector2<T>& min, const sf::Vector2<T>& max) noexcept
		{
			return sf::Vector2<T> {gen<T>(min.x, max.x), gen<T>(min.y, max.y)};
		}

		template<meta::is_arithmetic T>
		sf::Vector3<T> Random::gen_vec3(const sf::Vector3<T>& min, const sf::Vector3<T>& max) noexcept
		{
			return sf::Vector3<T> {gen<T>(min.x, max.x), gen<T>(min.y, max.y), gen<T>(min.z, max.z)};
		}
	} // namespace math
} // namespace galaxy

#endif
