///
/// Vector2.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Vector2.hpp"

namespace galaxy
{
	namespace math
	{
		Vector2::Vector2() noexcept
		{
			x = 0.0f;
			y = 0.0f;
		}

		Vector2::Vector2(const float x, const float y) noexcept
		{
			this->x = x;
			this->y = y;
		}

		Vector2::Vector2(const ::Vector2& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
		}

		Vector2::Vector2(const Vector2& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
		}

		Vector2::Vector2(Vector2&& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
		}

		Vector2& Vector2::operator=(const Vector2& vec) noexcept
		{
			if (this != &vec)
			{
				this->x = vec.x;
				this->y = vec.y;
			}

			return *this;
		}

		Vector2& Vector2::operator=(Vector2&& vec) noexcept
		{
			if (this != &vec)
			{
				this->x = vec.x;
				this->y = vec.y;
			}

			return *this;
		}

		Vector2::~Vector2() noexcept
		{
		}

		float Vector2::length() const noexcept
		{
			return Vector2Length(*this);
		}

		float Vector2::length_sqr() const noexcept
		{
			return Vector2LengthSqr(*this);
		}

		float Vector2::dot_product(const Vector2& v2) const noexcept
		{
			return Vector2DotProduct(*this, v2);
		}

		float Vector2::distance(const Vector2& v2) const noexcept
		{
			return Vector2Distance(*this, v2);
		}

		float Vector2::distance_sqr(const Vector2& v2) const noexcept
		{
			return Vector2DistanceSqr(*this, v2);
		}

		float Vector2::angle(const Vector2& v2) const noexcept
		{
			return Vector2Angle(*this, v2);
		}

		float Vector2::line_angle(const Vector2& v2) const noexcept
		{
			return Vector2LineAngle(Vector2Normalize(*this), Vector2Normalize(v2));
		}

		void Vector2::scale(const float scale) noexcept
		{
			set(Vector2Scale(*this, scale));
		}

		void Vector2::negate() noexcept
		{
			set(Vector2Negate(*this));
		}

		void Vector2::normalize() noexcept
		{
			set(Vector2Normalize(*this));
		}

		void Vector2::transform(const Matrix& mat) noexcept
		{
			set(Vector2Transform(*this, mat));
		}

		void Vector2::rotate(const float angle) noexcept
		{
			set(Vector2Rotate(*this, angle));
		}

		void Vector2::move_towards(const Vector2& target, const float max_distance) noexcept
		{
			set(Vector2MoveTowards(*this, target, max_distance));
		}

		void Vector2::invert() noexcept
		{
			set(Vector2Invert(*this));
		}

		void Vector2::clamp(const Vector2& min, const Vector2& max) noexcept
		{
			set(Vector2Clamp(*this, min, max));
		}

		void Vector2::clamp_value(const float min, const float max) noexcept
		{
			set(Vector2ClampValue(*this, min, max));
		}

		Vector2 Vector2::lerp(const Vector2& v1, const Vector2& v2, const float amount) noexcept
		{
			Vector2 result;
			result.set(Vector2Lerp(v1, v2, amount));

			return result;
		}

		Vector2 Vector2::reflect(const Vector2& v, const Vector2& normal) noexcept
		{
			Vector2 result;
			result.set(Vector2Reflect(v, normal));

			return result;
		}

		Vector2 Vector2::min(const Vector2& v2) noexcept
		{
			Vector2 result;
			result.set(Vector2Min(*this, v2));

			return result;
		}

		Vector2 Vector2::max(const Vector2& v2) noexcept
		{
			Vector2 result;
			result.set(Vector2Max(*this, v2));

			return result;
		}

		Vector2 Vector2::refract(const Vector2& n, const float r) noexcept
		{
			Vector2 result;
			result.set(Vector2Refract(*this, n, r));

			return result;
		}

		void Vector2::set(const ::Vector2& vec) noexcept
		{
			this->x = vec.x;
			this->y = vec.y;
		}

		Vector2 operator+(const Vector2& lhs, const Vector2& rhs) noexcept
		{
			return {lhs.x + rhs.x, lhs.y + rhs.y};
		}

		const Vector2& operator+=(Vector2& lhs, const Vector2& rhs) noexcept
		{
			lhs.x += rhs.x;
			lhs.y += rhs.y;

			return lhs;
		}

		Vector2 operator-(const Vector2& lhs, const Vector2& rhs) noexcept
		{
			return {lhs.x - rhs.x, lhs.y - rhs.y};
		}

		const Vector2& operator-=(Vector2& lhs, const Vector2& rhs) noexcept
		{
			lhs.x -= rhs.x;
			lhs.y -= rhs.y;

			return lhs;
		}

		Vector2 operator*(const Vector2& lhs, const float& rhs) noexcept
		{
			return {lhs.x * rhs, lhs.y * rhs};
		}

		const Vector2& operator*=(Vector2& lhs, const float& rhs) noexcept
		{
			lhs.x *= rhs;
			lhs.y *= rhs;

			return lhs;
		}

		Vector2 operator*(const Vector2& lhs, const Vector2& rhs) noexcept
		{
			return {lhs.x * rhs.x, lhs.y * rhs.y};
		}

		const Vector2& operator*=(Vector2& lhs, const Vector2& rhs) noexcept
		{
			lhs.x *= rhs.x;
			lhs.y *= rhs.y;

			return lhs;
		}

		Vector2 operator/(const Vector2& lhs, const float& rhs) noexcept
		{
			return {lhs.x / rhs, lhs.y / rhs};
		}

		const Vector2& operator/=(Vector2& lhs, const float& rhs) noexcept
		{
			lhs.x /= rhs;
			lhs.y /= rhs;

			return lhs;
		}

		Vector2 operator/(const Vector2& lhs, const Vector2& rhs) noexcept
		{
			return {lhs.x / rhs.x, lhs.y / rhs.y};
		}

		const Vector2& operator/=(Vector2& lhs, const Vector2& rhs) noexcept
		{
			lhs.x /= rhs.x;
			lhs.y /= rhs.y;

			return lhs;
		}

		bool operator==(const Vector2& lhs, const Vector2& rhs) noexcept
		{
			return (lhs.x == rhs.x) && (lhs.y == rhs.y);
		}

		bool operator!=(const Vector2& lhs, const Vector2& rhs) noexcept
		{
			return (lhs.x != rhs.x) || (lhs.y != rhs.y);
		}
	} // namespace math
} // namespace galaxy
