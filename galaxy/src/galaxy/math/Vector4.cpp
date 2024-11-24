///
/// Vector4.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Vector4.hpp"

namespace galaxy
{
	namespace math
	{
		Vector4::Vector4() noexcept
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}

		Vector4::Vector4(const float x, const float y, const float z, const float w) noexcept
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Vector4::Vector4(const ::Vector4& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
			this->w = vec.w;
		}

		Vector4::Vector4(const Vector4& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
			this->w = vec.w;
		}

		Vector4::Vector4(Vector4&& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
			this->w = vec.w;
		}

		Vector4& Vector4::operator=(const Vector4& vec) noexcept
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

		Vector4& Vector4::operator=(Vector4&& vec) noexcept
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

		Vector4::~Vector4() noexcept
		{
		}

		float Vector4::length() const noexcept
		{
			return Vector4Length(*this);
		}

		float Vector4::length_sqr() const noexcept
		{
			return Vector4LengthSqr(*this);
		}

		float Vector4::dot_product(const Vector4& v2) const noexcept
		{
			return Vector4DotProduct(*this, v2);
		}

		float Vector4::distance(const Vector4& v2) const noexcept
		{
			return Vector4Distance(*this, v2);
		}

		float Vector4::distance_sqr(const Vector4& v2) const noexcept
		{
			return Vector4DistanceSqr(*this, v2);
		}

		void Vector4::scale(const float scale) noexcept
		{
			set(Vector4Scale(*this, scale));
		}

		void Vector4::negate() noexcept
		{
			set(Vector4Negate(*this));
		}

		void Vector4::normalize() noexcept
		{
			set(Vector4Normalize(*this));
		}

		void Vector4::move_towards(const Vector4& target, const float max_distance) noexcept
		{
			set(Vector4MoveTowards(*this, target, max_distance));
		}

		void Vector4::invert() noexcept
		{
			set(Vector4Invert(*this));
		}

		Vector4 Vector4::lerp(const Vector4& v2, const float amount) noexcept
		{
			Vector4 result;
			result.set(Vector4Lerp(*this, v2, amount));

			return result;
		}

		Vector4 Vector4::min(const Vector4& v2) noexcept
		{
			Vector4 result;
			result.set(Vector4Min(*this, v2));

			return result;
		}

		Vector4 Vector4::max(const Vector4& v2) noexcept
		{
			Vector4 result;
			result.set(Vector4Max(*this, v2));

			return result;
		}

		void Vector4::set(const ::Vector4& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
			this->w = vec.w;
		}

		Vector4 operator+(const Vector4& lhs, const Vector4& rhs) noexcept
		{
			return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
		}

		const Vector4& operator+=(Vector4& lhs, const Vector4& rhs) noexcept
		{
			lhs.x += rhs.x;
			lhs.y += rhs.y;
			lhs.z += rhs.z;
			lhs.w += rhs.w;

			return lhs;
		}

		Vector4 operator-(const Vector4& lhs, const Vector4& rhs) noexcept
		{
			return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
		}

		const Vector4& operator-=(Vector4& lhs, const Vector4& rhs) noexcept
		{
			lhs.x -= rhs.x;
			lhs.y -= rhs.y;
			lhs.z -= rhs.z;
			lhs.w -= rhs.w;

			return lhs;
		}

		Vector4 operator*(const Vector4& lhs, const float& rhs) noexcept
		{
			return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs};
		}

		const Vector4& operator*=(Vector4& lhs, const float& rhs) noexcept
		{
			lhs.x *= rhs;
			lhs.y *= rhs;
			lhs.z *= rhs;
			lhs.w *= rhs;

			return lhs;
		}

		Vector4 operator*(const Vector4& lhs, const Vector4& rhs) noexcept
		{
			return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w};
		}

		const Vector4& operator*=(Vector4& lhs, const Vector4& rhs) noexcept
		{
			lhs.x *= rhs.x;
			lhs.y *= rhs.y;
			lhs.z *= rhs.z;
			lhs.w *= rhs.w;

			return lhs;
		}

		Vector4 operator/(const Vector4& lhs, const float& rhs) noexcept
		{
			return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs};
		}

		const Vector4& operator/=(Vector4& lhs, const float& rhs) noexcept
		{
			lhs.x /= rhs;
			lhs.y /= rhs;
			lhs.z /= rhs;
			lhs.w /= rhs;

			return lhs;
		}

		Vector4 operator/(const Vector4& lhs, const Vector4& rhs) noexcept
		{
			return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w};
		}

		const Vector4& operator/=(Vector4& lhs, const Vector4& rhs) noexcept
		{
			lhs.x /= rhs.x;
			lhs.y /= rhs.y;
			lhs.z /= rhs.z;
			lhs.w /= rhs.w;

			return lhs;
		}

		bool operator==(const Vector4& lhs, const Vector4& rhs) noexcept
		{
			return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
		}

		bool operator!=(const Vector4& lhs, const Vector4& rhs) noexcept
		{
			return (lhs.x != rhs.x) || (lhs.y != rhs.y) || (lhs.z != rhs.z) || (lhs.w != rhs.w);
		}
	} // namespace math
} // namespace galaxy
