///
/// Quaternion.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Quaternion.hpp"

namespace galaxy
{
	namespace math
	{
		Quaternion::Quaternion() noexcept
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}

		Quaternion::Quaternion(const float x, const float y, const float z, const float w) noexcept
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Quaternion::Quaternion(const ::Quaternion& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
			this->w = vec.w;
		}

		Quaternion::Quaternion(const Quaternion& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
			this->w = vec.w;
		}

		Quaternion::Quaternion(Quaternion&& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
			this->w = vec.w;
		}

		Quaternion& Quaternion::operator=(const Quaternion& vec) noexcept
		{
			if (this != &vec)
			{
				this->x = vec.x;
				this->y = vec.y;
				this->z = vec.z;
				this->w = vec.w;
			}

			return *this;
		}

		Quaternion& Quaternion::operator=(Quaternion&& vec) noexcept
		{
			if (this != &vec)
			{
				this->x = vec.x;
				this->y = vec.y;
				this->z = vec.z;
				this->w = vec.w;
			}

			return *this;
		}

		Quaternion::~Quaternion() noexcept
		{
		}

		float Quaternion::length() const noexcept
		{
			return QuaternionLength(*this);
		}

		void Quaternion::normalize() noexcept
		{
			set(QuaternionNormalize(*this));
		}

		void Quaternion::invert() noexcept
		{
			set(QuaternionInvert(*this));
		}

		void Quaternion::scale(const float scale) noexcept
		{
			set(QuaternionScale(*this, scale));
		}

		void Quaternion::lerp(const Quaternion& q2, const float amount) noexcept
		{
			set(QuaternionLerp(*this, q2, amount));
		}

		void Quaternion::nlerp(const Quaternion& q2, const float amount) noexcept
		{
			set(QuaternionNlerp(*this, q2, amount));
		}

		void Quaternion::slerp(const Quaternion& q2, const float amount) noexcept
		{
			set(QuaternionSlerp(*this, q2, amount));
		}

		void Quaternion::transform(const Matrix& mat) noexcept
		{
			set(QuaternionTransform(*this, mat.matrix()));
		}

		void Quaternion::cubic_hermite_spline(const Quaternion& out_tangent1, const Quaternion& q2, const Quaternion& in_tangent2, float t) noexcept
		{
			set(QuaternionCubicHermiteSpline(*this, out_tangent1, q2, in_tangent2, t));
		}

		void Quaternion::from_matrix(const Matrix& mat) noexcept
		{
			set(QuaternionFromMatrix(mat.matrix()));
		}

		Matrix Quaternion::to_matrix() noexcept
		{
			return {QuaternionToMatrix(*this)};
		}

		void Quaternion::from_axis_angle(const Vector3& axis, float angle) noexcept
		{
			set(QuaternionFromAxisAngle(axis, angle));
		}

		void Quaternion::to_axis_angle(Vector3* out_axis, float* out_angle) noexcept
		{
			QuaternionToAxisAngle(*this, out_axis, out_angle);
		}

		void Quaternion::from_euler(float pitch, float yaw, float roll) noexcept
		{
			set(QuaternionFromEuler(pitch, yaw, roll));
		}

		Vector3 Quaternion::to_euler() noexcept
		{
			return {QuaternionToEuler(*this)};
		}

		Quaternion Quaternion::from_vector3_to_vector3(const Vector3& from, const Vector3& to) noexcept
		{
			return {QuaternionFromVector3ToVector3(from, to)};
		}

		void Quaternion::set(const ::Quaternion& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
			this->w = vec.w;
		}

		Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
		}

		const Quaternion& operator+=(Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			lhs.x += rhs.x;
			lhs.y += rhs.y;
			lhs.z += rhs.z;
			lhs.w += rhs.w;

			return lhs;
		}

		Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
		}

		const Quaternion& operator-=(Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			lhs.x -= rhs.x;
			lhs.y -= rhs.y;
			lhs.z -= rhs.z;
			lhs.w -= rhs.w;

			return lhs;
		}

		Quaternion operator*(const Quaternion& lhs, const float& rhs) noexcept
		{
			return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs};
		}

		const Quaternion& operator*=(Quaternion& lhs, const float& rhs) noexcept
		{
			lhs.x *= rhs;
			lhs.y *= rhs;
			lhs.z *= rhs;
			lhs.w *= rhs;

			return lhs;
		}

		Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w};
		}

		const Quaternion& operator*=(Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			lhs.x *= rhs.x;
			lhs.y *= rhs.y;
			lhs.z *= rhs.z;
			lhs.w *= rhs.w;

			return lhs;
		}

		Quaternion operator/(const Quaternion& lhs, const float& rhs) noexcept
		{
			return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs};
		}

		const Quaternion& operator/=(Quaternion& lhs, const float& rhs) noexcept
		{
			lhs.x /= rhs;
			lhs.y /= rhs;
			lhs.z /= rhs;
			lhs.w /= rhs;

			return lhs;
		}

		Quaternion operator/(const Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w};
		}

		const Quaternion& operator/=(Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			lhs.x /= rhs.x;
			lhs.y /= rhs.y;
			lhs.z /= rhs.z;
			lhs.w /= rhs.w;

			return lhs;
		}

		bool operator==(const Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
		}

		bool operator!=(const Quaternion& lhs, const Quaternion& rhs) noexcept
		{
			return (lhs.x != rhs.x) || (lhs.y != rhs.y) || (lhs.z != rhs.z) || (lhs.w != rhs.w);
		}
	} // namespace math
} // namespace galaxy
