///
/// Vector3.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Vector3.hpp"

namespace galaxy
{
	namespace math
	{
		Vector3::Vector3() noexcept
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		Vector3::Vector3(const float x, const float y, const float z) noexcept
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector3::Vector3(const ::Vector3& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
		}

		Vector3::Vector3(const Vector3& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
		}

		Vector3::Vector3(Vector3&& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
		}

		Vector3& Vector3::operator=(const Vector3& vec) noexcept
		{
			if (this != &vec)
			{
				this->x = vec.x;
				this->y = vec.y;
				this->z = vec.z;
			}

			return *this;
		}

		Vector3& Vector3::operator=(Vector3&& vec) noexcept
		{
			if (this != &vec)
			{
				this->x = vec.x;
				this->y = vec.y;
				this->z = vec.z;
			}

			return *this;
		}

		Vector3::~Vector3() noexcept
		{
		}

		float Vector3::length() const noexcept
		{
			return Vector3Length(*this);
		}

		float Vector3::length_sqr() const noexcept
		{
			return Vector3LengthSqr(*this);
		}

		float Vector3::dot_product(const Vector3& v2) const noexcept
		{
			return Vector3DotProduct(*this, v2);
		}

		float Vector3::distance(const Vector3& v2) const noexcept
		{
			return Vector3Distance(*this, v2);
		}

		float Vector3::distance_sqr(const Vector3& v2) const noexcept
		{
			return Vector3DistanceSqr(*this, v2);
		}

		float Vector3::angle(const Vector3& v2) const noexcept
		{
			return Vector3Angle(*this, v2);
		}

		void Vector3::scale(const float scale) noexcept
		{
			set(Vector3Scale(*this, scale));
		}

		void Vector3::negate() noexcept
		{
			set(Vector3Negate(*this));
		}

		void Vector3::normalize() noexcept
		{
			set(Vector3Normalize(*this));
		}

		void Vector3::transform(const Matrix& mat) noexcept
		{
			set(Vector3Transform(*this, mat));
		}

		void Vector3::move_towards(const Vector3& target, const float max_distance) noexcept
		{
			set(Vector3MoveTowards(*this, target, max_distance));
		}

		void Vector3::invert() noexcept
		{
			set(Vector3Invert(*this));
		}

		void Vector3::clamp(const Vector3& min, const Vector3& max) noexcept
		{
			set(Vector3Clamp(*this, min, max));
		}

		void Vector3::clamp_value(const float min, const float max) noexcept
		{
			set(Vector3ClampValue(*this, min, max));
		}

		Vector3 Vector3::lerp(const Vector3& v1, const Vector3& v2, const float amount) noexcept
		{
			Vector3 result;
			result.set(Vector3Lerp(v1, v2, amount));

			return result;
		}

		Vector3 Vector3::reflect(const Vector3& v, const Vector3& normal) noexcept
		{
			Vector3 result;
			result.set(Vector3Reflect(v, normal));

			return result;
		}

		Vector3 Vector3::min(const Vector3& v2) noexcept
		{
			Vector3 result;
			result.set(Vector3Min(*this, v2));

			return result;
		}

		Vector3 Vector3::max(const Vector3& v2) noexcept
		{
			Vector3 result;
			result.set(Vector3Max(*this, v2));

			return result;
		}

		Vector3 Vector3::refract(const Vector3& n, const float r) noexcept
		{
			Vector3 result;
			result.set(Vector3Refract(*this, n, r));

			return result;
		}

		Vector3 Vector3::cross_product(const Vector3& v2) noexcept
		{
			Vector3 result;
			result.set(Vector3CrossProduct(*this, v2));

			return result;
		}

		Vector3 Vector3::perpendicular() noexcept
		{
			Vector3 result;
			result.set(Vector3Perpendicular(*this));

			return result;
		}

		void Vector3::ortho_normalize(Vector3* v2) noexcept
		{
			Vector3OrthoNormalize(this, v2);
		}

		Vector3 Vector3::rotate_by_quaternion(const Quaternion& q) noexcept
		{
			Vector3 result;
			result.set(Vector3RotateByQuaternion(*this, q));

			return result;
		}

		Vector3 Vector3::rotate_by_axis_angle(const Vector3& axis, const float angle) noexcept
		{
			Vector3 result;
			result.set(Vector3RotateByAxisAngle(*this, axis, angle));

			return result;
		}

		Vector3 Vector3::barycenter(const Vector3& a, const Vector3& b, const Vector3& c) noexcept
		{
			Vector3 result;
			result.set(Vector3Barycenter(*this, a, b, c));

			return result;
		}

		Vector3 Vector3::unproject(const Matrix& projection, const Matrix& view) noexcept
		{
			Vector3 result;
			result.set(Vector3Unproject(*this, projection, view));

			return result;
		}

		Vector3 Vector3::project(const Vector3& v2) noexcept
		{
			Vector3 result;
			result.set(Vector3Project(*this, v2));

			return result;
		}

		Vector3 Vector3::reject(const Vector3& v2) noexcept
		{
			Vector3 result;
			result.set(Vector3Reject(*this, v2));

			return result;
		}

		Vector3 Vector3::cubic_hermite(const Vector3& tangent1, const Vector3& v2, const Vector3& tangent2, const float amount) noexcept
		{
			Vector3 result;
			result.set(Vector3CubicHermite(*this, tangent1, v2, tangent2, amount));

			return result;
		}

		void Vector3::set(const ::Vector3& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
		}

		Vector3 operator+(const Vector3& lhs, const Vector3& rhs) noexcept
		{
			return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
		}

		const Vector3& operator+=(Vector3& lhs, const Vector3& rhs) noexcept
		{
			lhs.x += rhs.x;
			lhs.y += rhs.y;
			lhs.z += rhs.z;

			return lhs;
		}

		Vector3 operator-(const Vector3& lhs, const Vector3& rhs) noexcept
		{
			return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
		}

		const Vector3& operator-=(Vector3& lhs, const Vector3& rhs) noexcept
		{
			lhs.x -= rhs.x;
			lhs.y -= rhs.y;
			lhs.z -= rhs.z;

			return lhs;
		}

		Vector3 operator*(const Vector3& lhs, const float& rhs) noexcept
		{
			return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
		}

		const Vector3& operator*=(Vector3& lhs, const float& rhs) noexcept
		{
			lhs.x *= rhs;
			lhs.y *= rhs;
			lhs.z *= rhs;

			return lhs;
		}

		Vector3 operator*(const Vector3& lhs, const Vector3& rhs) noexcept
		{
			return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
		}

		const Vector3& operator*=(Vector3& lhs, const Vector3& rhs) noexcept
		{
			lhs.x *= rhs.x;
			lhs.y *= rhs.y;
			lhs.z *= rhs.z;

			return lhs;
		}

		Vector3 operator/(const Vector3& lhs, const float& rhs) noexcept
		{
			return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
		}

		const Vector3& operator/=(Vector3& lhs, const float& rhs) noexcept
		{
			lhs.x /= rhs;
			lhs.y /= rhs;
			lhs.z /= rhs;

			return lhs;
		}

		Vector3 operator/(const Vector3& lhs, const Vector3& rhs) noexcept
		{
			return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};
		}

		const Vector3& operator/=(Vector3& lhs, const Vector3& rhs) noexcept
		{
			lhs.x /= rhs.x;
			lhs.y /= rhs.y;
			lhs.z /= rhs.z;

			return lhs;
		}

		bool operator==(const Vector3& lhs, const Vector3& rhs) noexcept
		{
			return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
		}

		bool operator!=(const Vector3& lhs, const Vector3& rhs) noexcept
		{
			return (lhs.x != rhs.x) || (lhs.y != rhs.y) || (lhs.z != rhs.z);
		}
	} // namespace math
} // namespace galaxy
