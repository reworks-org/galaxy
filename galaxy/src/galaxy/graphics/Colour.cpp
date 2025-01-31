///
/// Colour.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Colour.hpp"

namespace galaxy
{
	namespace graphics
	{
		Colour::Colour() noexcept
		{
			r = 0;
			g = 0;
			b = 0;
			a = 255;
		}

		Colour::Colour(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha) noexcept
		{
			r = red;
			g = green;
			b = blue;
			a = alpha;
		}

		Colour::Colour(Colour&& c) noexcept
		{
			this->r = c.r;
			this->g = c.g;
			this->b = c.b;
			this->a = c.a;
		}

		Colour& Colour::operator=(Colour&& c) noexcept
		{
			if (this != &c)
			{
				this->r = c.r;
				this->g = c.g;
				this->b = c.b;
				this->a = c.a;
			}

			return *this;
		}

		Colour::Colour(const Colour& c) noexcept
		{
			this->r = c.r;
			this->g = c.g;
			this->b = c.b;
			this->a = c.a;
		}

		Colour& Colour::operator=(const Colour& c) noexcept
		{
			if (this != &c)
			{
				this->r = c.r;
				this->g = c.g;
				this->b = c.b;
				this->a = c.a;
			}

			return *this;
		}

		Colour::~Colour() noexcept
		{
		}

		void Colour::fade(const float alpha) noexcept
		{
			const auto colour = ::Fade(*this, alpha);
			set(colour);
		}

		int Colour::to_hex() noexcept
		{
			return ::ColorToInt(*this);
		}

		void Colour::from_hex(const unsigned int hex_value) noexcept
		{
			const auto colour = ::GetColor(hex_value);
			set(colour);
		}

		math::Vector4 Colour::normalize() noexcept
		{
			const auto vec4 = ::ColorNormalize(*this);
			return math::Vector4(vec4.x, vec4.y, vec4.z, vec4.w);
		}

		void Colour::from_normalized(const math::Vector4& normalized) noexcept
		{
			const auto colour = ::ColorFromNormalized(normalized);
			set(colour);
		}

		math::Vector3 Colour::to_hsv() noexcept
		{
			const auto vec3 = ::ColorToHSV(*this);
			return math::Vector3(vec3.x, vec3.y, vec3.z);
		}

		void Colour::from_hsv(const float hue, const float saturation, const float value) noexcept
		{
			const auto colour = ::ColorFromHSV(hue, saturation, value);
			set(colour);
		}

		void Colour::set_tint(const Colour& tint) noexcept
		{
			const auto colour = ::ColorTint(*this, tint);
			set(colour);
		}

		void Colour::set_brightness(const float factor) noexcept
		{
			const auto colour = ::ColorBrightness(*this, factor);
			set(colour);
		}

		void Colour::set_contrast(const float contrast) noexcept
		{
			const auto colour = ::ColorContrast(*this, contrast);
			set(colour);
		}

		void Colour::set_alpha(const float alpha) noexcept
		{
			const auto colour = ::ColorAlpha(*this, alpha);
			set(colour);
		}

		void Colour::alpha_blend(const Colour& src, const Colour& tint) noexcept
		{
			const auto colour = ::ColorAlphaBlend(*this, src, tint);
			set(colour);
		}

		void Colour::lerp(const Colour& colour2, const float factor) noexcept
		{
			const auto colour = ::ColorLerp(*this, colour2, factor);
			set(colour);
		}

		void Colour::set(const ::Color colour) noexcept
		{
			r = colour.r;
			g = colour.g;
			b = colour.b;
			a = colour.a;
		}

		bool operator==(const Colour& lhs, const Colour& rhs) noexcept
		{
			return ::ColorIsEqual(lhs, rhs);
		}

		bool operator!=(const Colour& lhs, const Colour& rhs) noexcept
		{
			return !::ColorIsEqual(lhs, rhs);
		}
	} // namespace graphics
} // namespace galaxy
