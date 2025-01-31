///
/// Matrix.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_MATRIX_HPP_
#define GALAXY_MATH_MATRIX_HPP_

#include "galaxy/platform/Pragma.hpp"
#include "galaxy/math/Vector3.hpp"

namespace galaxy
{
	namespace math
	{
		///
		/// Matrix type.
		///
		class Matrix final
		{
		public:
			///
			/// Constructor.
			///
			Matrix() noexcept;

			///
			/// Raylib matrix constructor.
			///
			/// \param matrix Matrix to construct from.
			///
			Matrix(const ::Matrix& matrix) noexcept;

			///
			/// Copy constructor.
			///
			Matrix(const Matrix&) noexcept;

			///
			/// Move constructor.
			///
			Matrix(Matrix&&) noexcept;

			///
			/// Copy assignment operator.
			///
			[[nodiscard]]
			Matrix& operator=(const Matrix&) noexcept;

			///
			/// Move assignment operator.
			///
			[[nodiscard]]
			Matrix& operator=(Matrix&&) noexcept;

			///
			/// Destructor.
			///
			~Matrix() noexcept;

			///
			/// Compute matrix determinant.
			///
			/// \return The determinant of the matrix.
			///
			[[nodiscard]]
			float determinant() const noexcept;

			///
			/// Get the trace of the matrix (sum of the values along the diagonal).
			///
			/// \return The trace of the matrix.
			///
			[[nodiscard]]
			float trace() const noexcept;

			///
			/// Transpose the provided matrix.
			///
			void transpose() noexcept;

			///
			/// Invert the provided matrix.
			///
			void invert() noexcept;

			///
			/// Get translation matrix.
			///
			/// \param x The translation value along the x-axis.
			/// \param y The translation value along the y-axis.
			/// \param z The translation value along the z-axis.
			///
			void translate(const float x, const float y, const float z) noexcept;

			///
			/// Create rotation matrix from axis and angle.
			///
			/// \param axis The rotation axis.
			/// \param angle The rotation angle (in radians).
			///
			void rotate(const Vector3& axis, const float angle) noexcept;

			///
			/// Get x-rotation matrix.
			///
			/// \param angle The rotation angle (in radians).
			///
			void rotate_x(const float angle) noexcept;

			///
			/// Get y-rotation matrix.
			///
			/// \param angle The rotation angle (in radians).
			///
			void rotate_y(const float angle) noexcept;

			///
			/// Get z-rotation matrix.
			///
			/// \param angle The rotation angle (in radians).
			///
			void rotate_z(const float angle) noexcept;

			///
			/// Get xyz-rotation matrix.
			///
			/// \param angle The rotation angle as a Vector3 (in radians).
			///
			void rotate_xyz(const Vector3& angle) noexcept;

			///
			/// Get zyx-rotation matrix.
			///
			/// \param angle The rotation angle as a Vector3 (in radians).
			///
			void rotate_zyx(const Vector3& angle) noexcept;

			///
			/// Get scaling matrix.
			///
			/// \param x The scaling value along the x-axis.
			/// \param y The scaling value along the y-axis.
			/// \param z The scaling value along the z-axis.
			///
			void scale(const float x, const float y, const float z) noexcept;

			///
			/// Get perspective projection matrix.
			///
			/// \param left The left value of the frustum.
			/// \param right The right value of the frustum.
			/// \param bottom The bottom value of the frustum.
			/// \param top The top value of the frustum.
			/// \param near The near value of the frustum.
			/// \param far The far value of the frustum.
			///
			/// \return The perspective projection matrix.
			///
			[[nodiscard]]
			static Matrix frustum(const double left, const double right, const double bottom, const double top, const double near, const double far) noexcept;

			///
			/// Get perspective projection matrix.
			///
			/// \param fovY The field of view angle in the y direction (in radians).
			/// \param aspect The aspect ratio (width/height).
			/// \param near The near clipping plane.
			/// \param far The far clipping plane.
			///
			/// \return The perspective projection matrix.
			///
			[[nodiscard]]
			static Matrix perspective(const double fovY, const double aspect, const double near, const double far) noexcept;

			///
			/// Get orthographic projection matrix.
			///
			/// \param left The left value of the view volume.
			/// \param right The right value of the view volume.
			/// \param bottom The bottom value of the view volume.
			/// \param top The top value of the view volume.
			/// \param near The near value of the view volume.
			/// \param far The far value of the view volume.
			///
			/// \return The orthographic projection matrix.
			///
			[[nodiscard]]
			static Matrix ortho(const double left, const double right, const double bottom, const double top, const double near, const double far) noexcept;

			///
			/// Get camera look-at matrix (view matrix).
			///
			/// \param eye The position of the camera.
			/// \param target The target position the camera is looking at.
			/// \param up The up vector.
			///
			/// \return The view matrix.
			///
			[[nodiscard]]
			static Matrix look_at(const Vector3& eye, const Vector3& target, const Vector3& up) noexcept;

			///
			/// Decompose a transformation matrix into its rotational, translational, and scaling components.
			///
			/// \param translation The output translation vector.
			/// \param rotation The output rotation quaternion.
			/// \param scale The output scale vector.
			///
			void decompose(Vector3* translation, Quaternion* rotation, Vector3* scale) const noexcept;

			///
			/// Set from raylib matrix.
			///
			/// \param matrix Matrix to set from.
			///
			void matrix(const ::Matrix& matrix) noexcept;

			///
			/// Get raylib matrix.
			///
			/// \return Raylib matrix reference.
			///
			[[nodiscard]]
			const ::Matrix& matrix() const noexcept;

		private:
			///
			/// Raylib matrix.
			///
			::Matrix m_matrix;
		};

		///
		/// Addition operator for Matrix.
		///
		/// \param lhs The left-hand side matrix.
		/// \param rhs The right-hand side matrix.
		///
		/// \return The resulting matrix from the addition.
		///
		[[nodiscard]]
		Matrix operator+(const Matrix& lhs, const Matrix& rhs) noexcept;

		///
		/// Addition assignment operator for Matrix.
		///
		/// \param lhs The left-hand side matrix.
		/// \param rhs The right-hand side matrix.
		///
		/// \return A reference to the left-hand side matrix after addition.
		///
		[[nodiscard]]
		const Matrix& operator+=(Matrix& lhs, const Matrix& rhs) noexcept;

		///
		/// Subtraction operator for Matrix.
		///
		/// \param lhs The left-hand side matrix.
		/// \param rhs The right-hand side matrix.
		///
		/// \return The resulting matrix from the subtraction.
		///
		[[nodiscard]]
		Matrix operator-(const Matrix& lhs, const Matrix& rhs) noexcept;

		///
		/// Subtraction assignment operator for Matrix.
		///
		/// \param lhs The left-hand side matrix.
		/// \param rhs The right-hand side matrix.
		///
		/// \return A reference to the left-hand side matrix after subtraction.
		///
		[[nodiscard]]
		const Matrix& operator-=(Matrix& lhs, const Matrix& rhs) noexcept;

		///
		/// Multiplication operator for two Matrixs.
		///
		/// \param lhs The left-hand side matrix.
		/// \param rhs The right-hand side matrix.
		///
		/// \return The resulting matrix from the multiplication.
		///
		[[nodiscard]]
		Matrix operator*(const Matrix& lhs, const Matrix& rhs) noexcept;

		///
		/// Multiplication assignment operator for two Matrixs.
		///
		/// \param lhs The left-hand side matrix.
		/// \param rhs The right-hand side matrix.
		///
		/// \return A reference to the left-hand side matrix after multiplication.
		///
		[[nodiscard]]
		const Matrix& operator*=(Matrix& lhs, const Matrix& rhs) noexcept;
	} // namespace math
} // namespace galaxy

#endif
