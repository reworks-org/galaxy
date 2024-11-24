///
/// Random.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_RANDOM_HPP_
#define GALAXY_MATH_RANDOM_HPP_

#include <random>

#include "galaxy/utils/Concepts.hpp"

#include "galaxy/math/Vector2.hpp"
#include "galaxy/math/Vector3.hpp"
#include "galaxy/math/Vector4.hpp"

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
			/// \tparam Type The type of number to return. Must be arithmetic.
			///
			/// \param min Minimum number inclusive.
			/// \param max Maximum number inclusive.
			///
			/// \return Returns number of the same type as inputs.
			///
			template<utils::is_arithmetic Type>
			[[nodiscard]]
			Type gen(const Type min, const Type max) noexcept;

			///
			/// Generate a random vec2.
			///
			/// \param min Minimum vec2 inclusive.
			/// \param max Maximum vec2 inclusive.
			///
			/// \return Pseudo-randomized vec2.
			///
			[[nodiscard]]
			Vector2 gen_vec2(const Vector2& min, const Vector2& max) noexcept;

			///
			/// Generate a random vec3.
			///
			/// \param min Minimum vec3 inclusive.
			/// \param max Maximum vec3 inclusive.
			///
			/// \return Pseudo-randomized vec3.
			///
			[[nodiscard]]
			Vector3 gen_vec3(const Vector3& min, const Vector3& max) noexcept;

			///
			/// Generate a random vec4.
			///
			/// \param min Minimum vec4 inclusive.
			/// \param max Maximum vec4 inclusive.
			///
			/// \return Pseudo-randomized vec4.
			///
			[[nodiscard]]
			Vector4 gen_vec4(const Vector4& min, const Vector4& max) noexcept;

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

		template<utils::is_arithmetic Type>
		inline Type Random::gen(const Type min, const Type max) noexcept
		{
			conditional_distribution<Type> dist {min, max};
			return dist(m_mt);
		}
	} // namespace math
} // namespace galaxy

#endif
