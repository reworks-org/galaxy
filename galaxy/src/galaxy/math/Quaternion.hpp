///
/// Quaternion.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_QUATERNION_HPP_
#define GALAXY_MATH_QUATERNION_HPP_

#include "galaxy/platform/Pragma.hpp"
#include "galaxy/math/Matrix.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26434)
#endif

namespace galaxy
{
	namespace math
	{
		///
		/// Quaternion type.
		///
		class Quaternion : public ::Quaternion
		{
		public:
			///
			/// Constructor.
			///
			Quaternion() noexcept;

			///
			/// Argument constructor.
			///
			/// \param x X value.
			/// \param y Y value.
			/// \param z Z value.
			/// \param w W value.
			///
			Quaternion(const float x, const float y, const float z, const float w) noexcept;

			///
			/// Raylib vector constructor.
			///
			/// \param vec Vector to construct from.
			///
			Quaternion(const ::Quaternion& vec) noexcept;

			///
			/// Copy constructor.
			///
			Quaternion(const Quaternion&) noexcept;

			///
			/// Move constructor.
			///
			Quaternion(Quaternion&&) noexcept;

			///
			/// Copy assignment operator.
			///
			[[nodiscard]]
			Quaternion& operator=(const Quaternion&) noexcept;

			///
			/// Move assignment operator.
			///
			[[nodiscard]]
			Quaternion& operator=(Quaternion&&) noexcept;

			///
			/// Destructor.
			///
			~Quaternion() noexcept;

			///
			/// Compute the length of a quaternion.
			///
			/// \return The length of the quaternion.
			///
			[[nodiscard]]
			float length() const noexcept;

			///
			/// Normalize the provided quaternion.
			///
			void normalize() noexcept;

			///
			/// Invert the provided quaternion.
			///
			void invert() noexcept;

			///
			/// Scale quaternion by float value.
			///
			/// \param mul The float value to scale by.
			///
			void scale(const float mul) noexcept;

			///
			/// Calculate linear interpolation between two quaternions.
			///
			/// \param q2 The second quaternion.
			/// \param amount The interpolation amount.
			///
			void lerp(const Quaternion& q2, const float amount) noexcept;

			///
			/// Calculate slerp-optimized interpolation between two quaternions.
			///
			/// \param q2 The second quaternion.
			/// \param amount The interpolation amount.
			///
			void nlerp(const Quaternion& q2, const float amount) noexcept;

			///
			/// Calculate spherical linear interpolation between two quaternions.
			///
			/// \param q2 The second quaternion.
			/// \param amount The interpolation amount.
			///
			void slerp(const Quaternion& q2, const float amount) noexcept;

			///
			/// Transform a quaternion given a transformation matrix.
			///
			/// \param mat The transformation matrix.
			///
			void transform(const Matrix& mat) noexcept;

			///
			/// Calculate quaternion cubic spline interpolation using Cubic Hermite Spline algorithm as described in the GLTF 2.0 specification.
			///
			/// \param out_tangent1 The outgoing tangent of the first quaternion.
			/// \param q2 The second quaternion.
			/// \param in_tangent2 The incoming tangent of the second quaternion.
			/// \param t The interpolation amount.
			///
			void cubic_hermite_spline(const Quaternion& out_tangent1, const Quaternion& q2, const Quaternion& in_tangent2, float t) noexcept;

			///
			/// Get a quaternion for a given rotation matrix.
			///
			/// \param mat The rotation matrix.
			///
			void from_matrix(const Matrix& mat) noexcept;

			///
			/// Get a matrix for a given quaternion.
			///
			/// \return The matrix representing the quaternion.
			///
			[[nodiscard]]
			Matrix to_matrix() noexcept;

			///
			/// Get rotation quaternion for an angle and axis.
			///
			/// \param axis The rotation axis.
			/// \param angle The rotation angle (in radians).
			///
			void from_axis_angle(const Vector3& axis, float angle) noexcept;

			///
			/// Get the rotation angle and axis for a given quaternion.
			///
			/// \param out_axis The output axis.
			/// \param out_angle The output angle.
			///
			void to_axis_angle(Vector3* out_axis, float* out_angle) noexcept;

			///
			/// Get the quaternion equivalent to Euler angles.
			///
			/// \param pitch The pitch angle.
			/// \param yaw The yaw angle.
			/// \param roll The roll angle.
			///
			void from_euler(float pitch, float yaw, float roll) noexcept;

			///
			/// Get the Euler angles equivalent to quaternion.
			///
			/// \return The Euler angles as a Vector3 (in radians).
			///
			[[nodiscard]]
			Vector3 to_euler() noexcept;

			///
			/// Calculate quaternion based on the rotation from one vector to another.
			///
			/// \param from The initial vector.
			/// \param to The target vector.
			///
			/// \return The resulting quaternion from the rotation.
			///
			[[nodiscard]]
			static Quaternion from_vector3_to_vector3(const Vector3& from, const Vector3& to) noexcept;

		private:
			///
			/// Set this vector from raylib vector.
			///
			/// \param vec Vector to construct from.
			///
			void set(const ::Quaternion& vec) noexcept;
		};

		///
		/// Addition operator for Quaternion.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the addition.
		///
		[[nodiscard]]
		Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs) noexcept;

		///
		/// Addition assignment operator for Quaternion.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after addition.
		///
		[[nodiscard]]
		const Quaternion& operator+=(Quaternion& lhs, const Quaternion& rhs) noexcept;

		///
		/// Subtraction operator for Quaternion.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the subtraction.
		///
		[[nodiscard]]
		Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs) noexcept;

		///
		/// Subtraction assignment operator for Quaternion.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after subtraction.
		///
		[[nodiscard]]
		const Quaternion& operator-=(Quaternion& lhs, const Quaternion& rhs) noexcept;

		///
		/// Multiplication operator for Quaternion with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return The resulting vector from the multiplication.
		///
		[[nodiscard]]
		Quaternion operator*(const Quaternion& lhs, const float& rhs) noexcept;

		///
		/// Multiplication assignment operator for Quaternion with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return A reference to the vector after multiplication.
		///
		[[nodiscard]]
		const Quaternion& operator*=(Quaternion& lhs, const float& rhs) noexcept;

		///
		/// Multiplication operator for two Quaternions.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the multiplication.
		///
		[[nodiscard]]
		Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs) noexcept;

		///
		/// Multiplication assignment operator for two Quaternions.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after multiplication.
		///
		[[nodiscard]]
		const Quaternion& operator*=(Quaternion& lhs, const Quaternion& rhs) noexcept;

		///
		/// Division operator for Quaternion with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return The resulting vector from the division.
		///
		[[nodiscard]]
		Quaternion operator/(const Quaternion& lhs, const float& rhs) noexcept;

		///
		/// Division assignment operator for Quaternion with a float.
		///
		/// \param lhs The vector.
		/// \param rhs The float value.
		///
		/// \return A reference to the vector after division.
		///
		[[nodiscard]]
		const Quaternion& operator/=(Quaternion& lhs, const float& rhs) noexcept;

		///
		/// Division operator for two Quaternions.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return The resulting vector from the division.
		///
		[[nodiscard]]
		Quaternion operator/(const Quaternion& lhs, const Quaternion& rhs) noexcept;

		///
		/// Division assignment operator for two Quaternions.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return A reference to the left-hand side vector after division.
		///
		[[nodiscard]]
		const Quaternion& operator/=(Quaternion& lhs, const Quaternion& rhs) noexcept;

		///
		/// Equality operator for Quaternion.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return True if the vectors are equal, false otherwise.
		///
		[[nodiscard]]
		bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept;

		///
		/// Inequality operator for Quaternion.
		///
		/// \param lhs The left-hand side vector.
		/// \param rhs The right-hand side vector.
		///
		/// \return True if the vectors are not equal, false otherwise.
		///
		[[nodiscard]]
		bool operator!=(const Quaternion& lhs, const Quaternion& rhs) noexcept;

	} // namespace math
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
