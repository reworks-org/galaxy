///
/// Vector3.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_VECTOR3_HPP_
#define GALAXY_MATH_VECTOR3_HPP_

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
		/// Vector3 type.
		///
		class Vector3 : public ::Vector3
		{
		public:
			///
			/// Constructor.
			///
			Vector3() noexcept;

			///
			/// Argument constructor.
			///
			/// \param x X value.
			/// \param y Y value.
			/// \param z Z value.
			///
			Vector3(const float x, const float y, const float z) noexcept;

			///
			/// Raylib vector constructor.
			///
			/// \param vec Vector to construct from.
			///
			Vector3(const ::Vector3& vec) noexcept;

			///
			/// Copy constructor.
			///
			Vector3(const Vector3&) noexcept;

			///
			/// Move constructor.
			///
			Vector3(Vector3&&) noexcept;

			///
			/// Copy assignment operator.
			///
			[[maybe_unused]]
			Vector3& operator=(const Vector3&) noexcept;

			///
			/// Move assignment operator.
			///
			[[maybe_unused]]
			Vector3& operator=(Vector3&&) noexcept;

			///
			/// Destructor.
			///
			~Vector3() noexcept;

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
			float dot_product(const Vector3& v2) const noexcept;

			///
			/// Calculate distance between two vectors.
			///
			/// \param v2 The second vector.
			///
			/// \return The distance between the two vectors.
			///
			[[nodiscard]]
			float distance(const Vector3& v2) const noexcept;

			///
			/// Calculate square distance between two vectors.
			///
			/// \param v2 The second vector.
			///
			/// \return The square distance between the two vectors.
			///
			[[nodiscard]]
			float distance_sqr(const Vector3& v2) const noexcept;

			///
			/// Calculate angle from two vectors.
			///
			/// \param v2 The second vector.
			///
			/// \return The angle between the two vectors.
			///
			[[nodiscard]]
			float angle(const Vector3& v2) const noexcept;

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
			/// Transform a Vector3 by a given Matrix.
			///
			/// \param mat The transformation matrix.
			///
			void transform(const Matrix& mat) noexcept;

			///
			/// Move Vector towards target.
			///
			/// \param target The target vector.
			/// \param max_distance The maximum distance to move.
			///
			void move_towards(const Vector3& target, const float max_distance) noexcept;

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
			void clamp(const Vector3& min, const Vector3& max) noexcept;

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
			static Vector3 lerp(const Vector3& v1, const Vector3& v2, const float amount) noexcept;

			///
			/// Calculate reflected vector to normal.
			///
			/// \param v The vector.
			/// \param normal The normal vector.
			///
			/// \return The reflected vector.
			///
			[[nodiscard]]
			static Vector3 reflect(const Vector3& v, const Vector3& normal) noexcept;

			///
			/// Get min value for each pair of components.
			///
			/// \param v2 The second vector.
			///
			/// \return The vector containing the minimum values for each pair of components.
			///
			[[nodiscard]]
			Vector3 min(const Vector3& v2) noexcept;

			///
			/// Get max value for each pair of components.
			///
			/// \param v2 The second vector.
			///
			/// \return The vector containing the maximum values for each pair of components.
			///
			[[nodiscard]]
			Vector3 max(const Vector3& v2) noexcept;

			///
			/// Compute the direction of a refracted ray.
			///
			/// \param n The normalized normal vector of the interface of two optical media.
			/// \param r The ratio of the refractive index of the medium from where the ray comes to the refractive index of the medium on the other side of the surface.
			///
			/// \return The direction of the refracted ray.
			///
			[[nodiscard]]
			Vector3 refract(const Vector3& n, const float r) noexcept;

			///
			/// Calculate two vectors cross product.
			///
			/// \param v2 The second vector.
			///
			/// \return The cross product of the two vectors.
			///
			[[nodiscard]]
			Vector3 cross_product(const Vector3& v2) noexcept;

			///
			/// Calculate one vector perpendicular vector.
			///
			/// \return The perpendicular vector.
			///
			[[nodiscard]]
			Vector3 perpendicular() noexcept;

			///
			/// Orthonormalize provided vectors. Makes vectors normalized and orthogonal to each other using Gram-Schmidt function implementation.
			///
			/// \param v2 Pointer to the second vector.
			///
			void ortho_normalize(Vector3* v2) noexcept;

			///
			/// Transform a vector by quaternion rotation.
			///
			/// \param q The quaternion.
			///
			/// \return The transformed vector.
			///
			[[nodiscard]]
			Vector3 rotate_by_quaternion(const Quaternion& q) noexcept;

			///
			/// Rotates a vector around an axis.
			///
			/// \param axis The rotation axis.
			/// \param angle The rotation angle (in radians).
			///
			/// \return The rotated vector.
			///
			[[nodiscard]]
			Vector3 rotate_by_axis_angle(const Vector3& axis, const float angle) noexcept;

			///
			/// Compute barycenter coordinates (u, v, w) for point p with respect to triangle (a, b, c). Assumes P is on the plane of the triangle.
			///
			/// \param a The first triangle vertex vector.
			/// \param b The second triangle vertex vector.
			/// \param c The third triangle vertex vector.
			///
			/// \return The barycenter coordinates as a vector.
			///
			[[nodiscard]]
			Vector3 barycenter(const Vector3& a, const Vector3& b, const Vector3& c) noexcept;

			///
			/// Projects a Vector3 from screen space into object space.
			///
			/// \param projection The projection matrix.
			/// \param view The view matrix.
			///
			/// \return The vector in object space.
			///
			[[nodiscard]]
			Vector3 unproject(const Matrix& projection, const Matrix& view) noexcept;

			///
			/// Calculate the projection of the vector v1 on to v2.
			///
			/// \param v2 The second vector.
			///
			/// \return The projection of v1 on to v2.
			///
			[[nodiscard]]
			Vector3 project(const Vector3& v2) noexcept;

			///
			/// Calculate the rejection of the vector v1 on to v2.
			///
			/// \param v2 The second vector.
			///
			/// \return The rejection of v1 on to v2.
			///
			[[nodiscard]]
			Vector3 reject(const Vector3& v2) noexcept;

			///
			/// Calculate cubic hermite interpolation between two vectors and their tangents as described in the GLTF 2.0 specification.
			///
			/// \param tangent1 The tangent at the first vector.
			/// \param v2 The second vector.
			/// \param tangent2 The tangent at the second vector.
			/// \param amount The interpolation amount.
			///
			/// \return The interpolated vector.
			///
			[[nodiscard]]
			Vector3 cubic_hermite(const Vector3& tangent1, const Vector3& v2, const Vector3& tangent2, const float amount) noexcept;

		private:
			///
			/// Set this vector from raylib vector.
			///
			/// \param vec Vector to construct from.
			///
			void set(const ::Vector3& vec) noexcept;
		};

		///
		/// Addition operator for Vector3.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the addition.
		///
		[[nodiscard]]
		Vector3 operator+(const Vector3& lhs, const Vector3& rhs) noexcept;

		///
		/// Addition assignment operator for Vector3.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after addition.
		///
		[[nodiscard]]
		const Vector3& operator+=(Vector3& lhs, const Vector3& rhs) noexcept;

		///
		/// Subtraction operator for Vector3.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the subtraction.
		///
		[[nodiscard]]
		Vector3 operator-(const Vector3& lhs, const Vector3& rhs) noexcept;

		///
		/// Subtraction assignment operator for Vector3.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after subtraction.
		///
		[[nodiscard]]
		const Vector3& operator-=(Vector3& lhs, const Vector3& rhs) noexcept;

		///
		/// Multiplication operator for Vector3 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return The resulting vector from the multiplication.
		///
		[[nodiscard]]
		Vector3 operator*(const Vector3& lhs, const float& rhs) noexcept;

		///
		/// Multiplication assignment operator for Vector3 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return A reference to the vector after multiplication.
		///
		[[nodiscard]]
		const Vector3& operator*=(Vector3& lhs, const float& rhs) noexcept;

		///
		/// Multiplication operator for two Vector3s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the multiplication.
		///
		[[nodiscard]]
		Vector3 operator*(const Vector3& lhs, const Vector3& rhs) noexcept;

		///
		/// Multiplication assignment operator for two Vector3s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after multiplication.
		///
		[[nodiscard]]
		const Vector3& operator*=(Vector3& lhs, const Vector3& rhs) noexcept;

		///
		/// Division operator for Vector3 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return The resulting vector from the division.
		///
		[[nodiscard]]
		Vector3 operator/(const Vector3& lhs, const float& rhs) noexcept;

		///
		/// Division assignment operator for Vector3 with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return A reference to the vector after division.
		///
		[[nodiscard]]
		const Vector3& operator/=(Vector3& lhs, const float& rhs) noexcept;

		///
		/// Division operator for two Vector3s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the division.
		///
		[[nodiscard]]
		Vector3 operator/(const Vector3& lhs, const Vector3& rhs) noexcept;

		///
		/// Division assignment operator for two Vector3s.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after division.
		///
		[[nodiscard]]
		const Vector3& operator/=(Vector3& lhs, const Vector3& rhs) noexcept;

		///
		/// Equality operator for Vector3.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return True if the vectors are equal, false otherwise.
		///
		[[nodiscard]]
		bool operator==(const Vector3& lhs, const Vector3& rhs) noexcept;

		///
		/// Inequality operator for Vector3.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return True if the vectors are not equal, false otherwise.
		///
		[[nodiscard]]
		bool operator!=(const Vector3& lhs, const Vector3& rhs) noexcept;
	} // namespace math
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
