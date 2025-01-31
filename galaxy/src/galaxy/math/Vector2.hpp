///
/// Vector2.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_VECTOR2_HPP_
#define GALAXY_MATH_VECTOR2_HPP_

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
		/// Vector2 type.
		///
		class Vector2 : public ::Vector2
		{
		public:
			///
			/// Constructor.
			///
			Vector2() noexcept;

			///
			/// Argument constructor.
			///
			/// \param x X value.
			/// \param y Y value.
			///
			Vector2(const float x, const float y) noexcept;

			///
			/// Raylib vector constructor.
			///
			/// \param vec Vector to construct from.
			///
			Vector2(const ::Vector2& vec) noexcept;

			///
			/// Copy constructor.
			///
			Vector2(const Vector2&) noexcept;

			///
			/// Move constructor.
			///
			Vector2(Vector2&&) noexcept;

			///
			/// Copy assignment operator.
			///
			[[nodiscard]]
			Vector2& operator=(const Vector2&) noexcept;

			///
			/// Move assignment operator.
			///
			[[nodiscard]]
			Vector2& operator=(Vector2&&) noexcept;

			///
			/// Destructor.
			///
			~Vector2() noexcept;

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
			float dot_product(const Vector2& v2) const noexcept;

			///
			/// Calculate distance between two vectors.
			///
			/// \param v2 The second vector.
			///
			/// \return The distance between the two vectors.
			///
			[[nodiscard]]
			float distance(const Vector2& v2) const noexcept;

			///
			/// Calculate square distance between two vectors.
			///
			/// \param v2 The second vector.
			///
			/// \return The square distance between the two vectors.
			///
			[[nodiscard]]
			float distance_sqr(const Vector2& v2) const noexcept;

			///
			/// Calculate angle from two vectors.
			///
			/// \param v2 The second vector.
			///
			/// \return The angle between the two vectors.
			///
			[[nodiscard]]
			float angle(const Vector2& v2) const noexcept;

			///
			/// Calculate angle defined by two vectors line.
			///
			/// \param v2 The second vector.
			///
			/// \return The line angle between the two vectors.
			///
			[[nodiscard]]
			float line_angle(const Vector2& v2) const noexcept;

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
			/// Transform a Vector2 by a given Matrix.
			///
			/// \param mat The transformation matrix.
			///
			void transform(const Matrix& mat) noexcept;

			///
			/// Rotate vector by angle.
			///
			/// \param angle The rotation angle (in degrees).
			///
			void rotate(const float angle) noexcept;

			///
			/// Move Vector towards target.
			///
			/// \param target The target vector.
			/// \param max_distance The maximum distance to move.
			///
			void move_towards(const Vector2& target, const float max_distance) noexcept;

			///
			/// Invert the given vector.
			///
			void invert() noexcept;

			///
			/// Clamp the components of the vector between min and max values specified by the given vectors.
			///
			/// \param min The minimum values vector.
			/// \param max The maximum values vector.
			///
			void clamp(const Vector2& min, const Vector2& max) noexcept;

			///
			/// Clamp the magnitude of the vector between two min and max values.
			///
			/// \param min The minimum value.
			/// \param max The maximum value.
			///
			void clamp_value(const float min, const float max) noexcept;

			///
			/// Calculate linear interpolation between two vectors.
			///
			/// \param v1 The first vector.
			/// \param v2 The second vector.
			/// \param amount The interpolation amount.
			///
			/// \return The interpolated vector.
			///
			[[nodiscard]]
			static Vector2 lerp(const Vector2& v1, const Vector2& v2, const float amount) noexcept;

			///
			/// Calculate reflected vector to normal.
			///
			/// \param v The vector.
			/// \param normal The normal vector.
			///
			/// \return The reflected vector.
			///
			[[nodiscard]]
			static Vector2 reflect(const Vector2& v, const Vector2& normal) noexcept;

			///
			/// Get min value for each pair of components.
			///
			/// \param v2 The second vector.
			///
			/// \return The vector containing the minimum values for each pair of components.
			///
			[[nodiscard]]
			Vector2 min(const Vector2& v2) noexcept;

			///
			/// Get max value for each pair of components.
			///
			/// \param v2 The second vector.
			///
			/// \return The vector containing the maximum values for each pair of components.
			///
			[[nodiscard]]
			Vector2 max(const Vector2& v2) noexcept;

			///
			/// Compute the direction of a refracted ray.
			///
			/// \param n The normalized normal vector of the interface of two optical media.
			/// \param r The ratio of the refractive index of the medium from where the ray comes to the refractive index of the medium on the other side of the surface.
			///
			/// \return The direction of the refracted ray.
			///
			[[nodiscard]]
			Vector2 refract(const Vector2& n, const float r) noexcept;

		private:
			///
			/// Set this vector from raylib vector.
			///
			/// \param vec Vector to construct from.
			///
			void set(const ::Vector2& vec) noexcept;
		};

		///
		/// Addition operator for Vector2.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the addition.
		///
		[[nodiscard]]
		Vector2 operator+(const Vector2& lhs, const Vector2& rhs) noexcept;

		///
		/// Addition assignment operator for Vector2.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after addition.
		///
		[[nodiscard]]
		const Vector2& operator+=(Vector2& lhs, const Vector2& rhs) noexcept;

		///
		/// Subtraction operator for Vector2.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the subtraction.
		///
		[[nodiscard]]
		Vector2 operator-(const Vector2& lhs, const Vector2& rhs) noexcept;

		///
		/// Subtraction assignment operator for Vector2.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after subtraction.
		///
		[[nodiscard]]
		const Vector2& operator-=(Vector2& lhs, const Vector2& rhs) noexcept;

		///
		/// Multiplication operator for Vector2 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return The resulting vector from the multiplication.
		///
		[[nodiscard]]
		Vector2 operator*(const Vector2& lhs, const float& rhs) noexcept;

		///
		/// Multiplication assignment operator for Vector2 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return A reference to the vector after multiplication.
		///
		[[nodiscard]]
		const Vector2& operator*=(Vector2& lhs, const float& rhs) noexcept;

		///
		/// Multiplication operator for two Vector2s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the multiplication.
		///
		[[nodiscard]]
		Vector2 operator*(const Vector2& lhs, const Vector2& rhs) noexcept;

		///
		/// Multiplication assignment operator for two Vector2s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after multiplication.
		///
		[[nodiscard]]
		const Vector2& operator*=(Vector2& lhs, const Vector2& rhs) noexcept;

		///
		/// Division operator for Vector2 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return The resulting vector from the division.
		///
		[[nodiscard]]
		Vector2 operator/(const Vector2& lhs, const float& rhs) noexcept;

		///
		/// Division assignment operator for Vector2 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return A reference to the vector after division.
		///
		[[nodiscard]]
		const Vector2& operator/=(Vector2& lhs, const float& rhs) noexcept;

		///
		/// Division operator for two Vector2s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the division.
		///
		[[nodiscard]]
		Vector2 operator/(const Vector2& lhs, const Vector2& rhs) noexcept;

		///
		/// Division assignment operator for two Vector2s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after division.
		///
		[[nodiscard]]
		const Vector2& operator/=(Vector2& lhs, const Vector2& rhs) noexcept;

		///
		/// Equality operator for Vector2.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return True if the vectors are equal, false otherwise.
		///
		[[nodiscard]]
		bool operator==(const Vector2& lhs, const Vector2& rhs) noexcept;

		///
		/// Inequality operator for Vector2.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return True if the vectors are not equal, false otherwise.
		///
		[[nodiscard]]
		bool operator!=(const Vector2& lhs, const Vector2& rhs) noexcept;

	} // namespace math
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
