///
/// Matrix.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Matrix.hpp"

namespace galaxy
{
	namespace math
	{
		Matrix::Matrix() noexcept
		{
			m_matrix = MatrixIdentity();
		}

		Matrix::Matrix(const ::Matrix& matrix) noexcept
		{
			this->matrix(matrix);
		}

		Matrix::Matrix(const Matrix& m) noexcept
		{
			this->m_matrix = m.m_matrix;
		}

		Matrix::Matrix(Matrix&& m) noexcept
		{
			this->m_matrix = std::move(m.m_matrix);
		}

		Matrix& Matrix::operator=(const Matrix& m) noexcept
		{
			if (this != &m)
			{
				this->m_matrix = m.m_matrix;
			}

			return *this;
		}

		Matrix& Matrix::operator=(Matrix&& m) noexcept
		{
			if (this != &m)
			{
				this->m_matrix = std::move(m.m_matrix);
			}

			return *this;
		}

		Matrix::~Matrix() noexcept
		{
		}

		float Matrix::determinant() const noexcept
		{
			return MatrixDeterminant(m_matrix);
		}

		float Matrix::trace() const noexcept
		{
			return MatrixTrace(m_matrix);
		}

		void Matrix::transpose() noexcept
		{
			m_matrix = MatrixTranspose(m_matrix);
		}

		void Matrix::invert() noexcept
		{
			m_matrix = MatrixTranspose(m_matrix);
		}

		void Matrix::translate(const float x, const float y, const float z) noexcept
		{
			m_matrix = MatrixTranslate(x, y, z);
		}

		void Matrix::rotate(const Vector3& axis, const float angle) noexcept
		{
			m_matrix = MatrixRotate(axis, angle);
		}

		void Matrix::rotate_x(const float angle) noexcept
		{
			m_matrix = MatrixRotateX(angle);
		}

		void Matrix::rotate_y(const float angle) noexcept
		{
			m_matrix = MatrixRotateY(angle);
		}

		void Matrix::rotate_z(const float angle) noexcept
		{
			m_matrix = MatrixRotateZ(angle);
		}

		void Matrix::rotate_xyz(const Vector3& angle) noexcept
		{
			m_matrix = MatrixRotateXYZ(angle);
		}

		void Matrix::rotate_zyx(const Vector3& angle) noexcept
		{
			m_matrix = MatrixRotateZYX(angle);
		}

		void Matrix::scale(const float x, const float y, const float z) noexcept
		{
			m_matrix = MatrixScale(x, y, z);
		}

		Matrix Matrix::frustum(const double left, const double right, const double bottom, const double top, const double near, const double far) noexcept
		{
			Matrix matrix {MatrixFrustum(left, right, bottom, top, near, far)};
			return matrix;
		}

		Matrix Matrix::perspective(const double fovY, const double aspect, const double near, const double far) noexcept
		{
			Matrix matrix {MatrixPerspective(fovY, aspect, near, far)};
			return matrix;
		}

		Matrix Matrix::ortho(const double left, const double right, const double bottom, const double top, const double near, const double far) noexcept
		{
			Matrix matrix {MatrixOrtho(left, right, bottom, top, near, far)};
			return matrix;
		}

		Matrix Matrix::look_at(const Vector3& eye, const Vector3& target, const Vector3& up) noexcept
		{
			Matrix matrix {MatrixLookAt(eye, target, up)};
			return matrix;
		}

		void Matrix::decompose(Vector3* translation, Quaternion* rotation, Vector3* scale) const noexcept
		{
			MatrixDecompose(m_matrix, translation, rotation, scale);
		}

		void Matrix::matrix(const ::Matrix& matrix) noexcept
		{
			m_matrix = matrix;
		}

		const ::Matrix& Matrix::matrix() const noexcept
		{
			return m_matrix;
		}

		Matrix operator+(const Matrix& lhs, const Matrix& rhs) noexcept
		{
			Matrix matrix {MatrixAdd(lhs.matrix(), rhs.matrix())};
			return matrix;
		}

		const Matrix& operator+=(Matrix& lhs, const Matrix& rhs) noexcept
		{
			lhs.matrix(MatrixAdd(lhs.matrix(), rhs.matrix()));
			return lhs;
		}

		Matrix operator-(const Matrix& lhs, const Matrix& rhs) noexcept
		{
			Matrix matrix {MatrixSubtract(lhs.matrix(), rhs.matrix())};
			return matrix;
		}

		const Matrix& operator-=(Matrix& lhs, const Matrix& rhs) noexcept
		{
			lhs.matrix(MatrixSubtract(lhs.matrix(), rhs.matrix()));
			return lhs;
		}

		Matrix operator*(const Matrix& lhs, const Matrix& rhs) noexcept
		{
			Matrix matrix {MatrixMultiply(lhs.matrix(), rhs.matrix())};
			return matrix;
		}

		const Matrix& operator*=(Matrix& lhs, const Matrix& rhs) noexcept
		{
			lhs.matrix(MatrixMultiply(lhs.matrix(), rhs.matrix()));
			return lhs;
		}
	} // namespace math
} // namespace galaxy
