///
/// Vector4.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_VECTOR4_HPP_
#define GALAXY_MATH_VECTOR4_HPP_

#include <raylib.hpp>

#include "galaxy/platform/Pragma.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26434)
#endif

namespace galaxy
{
	namespace math
	{
		///
		/// Vector4 type.
		///
		class Vector4 : public ::Vector4
		{
		public:
			///
			/// Constructor.
			///
			Vector4() noexcept;

			///
			/// Argument constructor.
			///
			/// \param x X value.
			/// \param y Y value.
			/// \param z Z value.
			/// \param w W value.
			///
			Vector4(const float x, const float y, const float z, const float w) noexcept;

			///
			/// Raylib vector constructor.
			///
			/// \param vec Vector to construct from.
			///
			Vector4(const ::Vector4& vec) noexcept;

			///
			/// Copy constructor.
			///
			Vector4(const Vector4&) noexcept;

			///
			/// Move constructor.
			///
			Vector4(Vector4&&) noexcept;

			///
			/// Copy assignment operator.
			///
			Vector4& operator=(const Vector4&) noexcept;

			///
			/// Move assignment operator.
			///
			Vector4& operator=(Vector4&&) noexcept;

			///
			/// Destructor.
			///
			~Vector4() noexcept;

			///
			/// Calculate vector length.
			///
			/// \return The length of the vector.
			///
			[[nodiscard]]
			float length() const noexcept;

			///
			/// Calculate vector square length.
			///
			/// \return The square length of the vector.
			///
			[[nodiscard]]
			float length_sqr() const noexcept;

			///
			/// Calculate two vectors dot product.
			///
			/// \param v2 The second vector.
			///
			/// \return The dot product of the two vectors.
			///
			[[nodiscard]]
			float dot_product(const Vector4& v2) const noexcept;

			///
			/// Calculate distance between two vectors.
			///
			/// \param v2 The second vector.
			///
			/// \return The distance between the two vectors.
			///
			[[nodiscard]]
			float distance(const Vector4& v2) const noexcept;

			///
			/// Calculate square distance between two vectors.
			///
			/// \param v2 The second vector.
			///
			/// \return The square distance between the two vectors.
			///
			[[nodiscard]]
			float distance_sqr(const Vector4& v2) const noexcept;

			///
			/// Scale vector (multiply by value).
			///
			/// \param scale The scale value.
			///
			void scale(const float scale) noexcept;

			///
			/// Negate vector.
			///
			void negate() noexcept;

			///
			/// Normalize provided vector.
			///
			void normalize() noexcept;

			///
			/// Move Vector towards target.
			///
			/// \param target The target vector.
			/// \param max_distance The maximum distance to move.
			///
			void move_towards(const Vector4& target, const float max_distance) noexcept;

			///
			/// Invert the given vector.
			///
			void invert() noexcept;

			///
			/// Calculate linear interpolation between two vectors.
			///
			/// \param v2 The second vector.
			/// \param amount The interpolation amount.
			///
			/// \return The interpolated vector.
			///
			[[nodiscard]]
			Vector4 lerp(const Vector4& v2, const float amount) noexcept;

			///
			/// Get min value for each pair of components.
			///
			/// \param v2 The second vector.
			///
			/// \return The vector containing the minimum values for each pair of components.
			///
			[[nodiscard]]
			Vector4 min(const Vector4& v2) noexcept;

			///
			/// Get max value for each pair of components.
			///
			/// \param v2 The second vector.
			///
			/// \return The vector containing the maximum values for each pair of components.
			///
			[[nodiscard]]
			Vector4 max(const Vector4& v2) noexcept;

		private:
			///
			/// Set this vector from raylib vector.
			///
			/// \param vec Vector to construct from.
			///
			void set(const ::Vector4& vec) noexcept;
		};

		///
		/// Addition operator for Vector4.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the addition.
		///
		Vector4 operator+(const Vector4& lhs, const Vector4& rhs) noexcept;

		///
		/// Addition assignment operator for Vector4.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after addition.
		///
		const Vector4& operator+=(Vector4& lhs, const Vector4& rhs) noexcept;

		///
		/// Subtraction operator for Vector4.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the subtraction.
		///
		Vector4 operator-(const Vector4& lhs, const Vector4& rhs) noexcept;

		///
		/// Subtraction assignment operator for Vector4.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after subtraction.
		///
		const Vector4& operator-=(Vector4& lhs, const Vector4& rhs) noexcept;

		///
		/// Multiplication operator for Vector4 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return The resulting vector from the multiplication.
		///
		Vector4 operator*(const Vector4& lhs, const float& rhs) noexcept;

		///
		/// Multiplication assignment operator for Vector4 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return A reference to the vector after multiplication.
		///
		const Vector4& operator*=(Vector4& lhs, const float& rhs) noexcept;

		///
		/// Multiplication operator for two Vector4s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the multiplication.
		///
		Vector4 operator*(const Vector4& lhs, const Vector4& rhs) noexcept;

		///
		/// Multiplication assignment operator for two Vector4s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after multiplication.
		///
		const Vector4& operator*=(Vector4& lhs, const Vector4& rhs) noexcept;

		///
		/// Division operator for Vector4 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return The resulting vector from the division.
		///
		Vector4 operator/(const Vector4& lhs, const float& rhs) noexcept;

		///
		/// Division assignment operator for Vector4 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return A reference to the vector after division.
		///
		const Vector4& operator/=(Vector4& lhs, const float& rhs) noexcept;

		///
		/// Division operator for two Vector4s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the division.
		///
		Vector4 operator/(const Vector4& lhs, const Vector4& rhs) noexcept;

		///
		/// Division assignment operator for two Vector4s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after division.
		///
		const Vector4& operator/=(Vector4& lhs, const Vector4& rhs) noexcept;

		///
		/// Equality operator for Vector4.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return True if the vectors are equal, false otherwise.
		///
		bool operator==(const Vector4& lhs, const Vector4& rhs) noexcept;

		///
		/// Inequality operator for Vector4.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return True if the vectors are not equal, false otherwise.
		///
		bool operator!=(const Vector4& lhs, const Vector4& rhs) noexcept;

	} // namespace math
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
