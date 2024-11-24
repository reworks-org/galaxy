///
/// Rectangle.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Rectangle.hpp"

namespace galaxy
{
	namespace graphics
	{
		Rectangle::Rectangle() noexcept
		{
			x      = 0.0f;
			y      = 0.0f;
			width  = 0.0f;
			height = 0.0f;
		}

		Rectangle::Rectangle(const float x, const float y, const float width, const float height) noexcept
		{
			this->x      = x;
			this->y      = y;
			this->width  = width;
			this->height = height;
		}

		Rectangle::Rectangle(const ::Rectangle& rect) noexcept
		{
			this->x      = rect.x;
			this->y      = rect.y;
			this->width  = rect.width;
			this->height = rect.height;
		}

		Rectangle::Rectangle(const Rectangle& rect) noexcept
		{
			this->x      = rect.x;
			this->y      = rect.y;
			this->width  = rect.width;
			this->height = rect.height;
		}

		Rectangle::Rectangle(Rectangle&& rect) noexcept
		{
			this->x      = rect.x;
			this->y      = rect.y;
			this->width  = rect.width;
			this->height = rect.height;
		}

		Rectangle& Rectangle::operator=(const Rectangle& rect) noexcept
		{
			if (this != &rect)
			{
				this->x      = rect.x;
				this->y      = rect.y;
				this->width  = rect.width;
				this->height = rect.height;
			}

			return *this;
		}

		Rectangle& Rectangle::operator=(Rectangle&& rect) noexcept
		{
			if (this != &rect)
			{
				this->x      = rect.x;
				this->y      = rect.y;
				this->width  = rect.width;
				this->height = rect.height;
			}

			return *this;
		}

		Rectangle::~Rectangle() noexcept
		{
		}

		bool Rectangle::check_collision_point_rec(const Vector2& point) noexcept
		{
			return CheckCollisionPointRec(point, *this);
		}

		Rectangle Rectangle::get_collision_rec(const Rectangle& rec2) noexcept
		{
			return {GetCollisionRec(*this, rec2)};
		}

		Vector4 Rectangle::to_vec4() noexcept
		{
			return {x, y, width, height};
		}
	} // namespace graphics
} // namespace galaxy
