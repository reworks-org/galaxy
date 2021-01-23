///
/// BatchedSprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include <nlohmann/json.hpp>

#include "BatchedSprite.hpp"

namespace galaxy
{
	namespace components
	{
		BatchedSprite::BatchedSprite() noexcept
		    : m_opacity {1.0f}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_offset {0}, m_z_level {0}
		{
		}

		BatchedSprite::BatchedSprite(const nlohmann::json& json)
		    : m_opacity {1.0f}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_offset {0}, m_z_level {0}
		{
			float opacity = json.at("opacity");
			m_opacity     = std::clamp(opacity, 0.0f, 1.0f);
			create({json.at("texture-x"), json.at("texture-y"), json.at("w"), json.at("h")});
		}

		BatchedSprite::BatchedSprite(BatchedSprite&& bs) noexcept
		{
			this->m_offset  = bs.m_offset;
			this->m_opacity = bs.m_opacity;
			this->m_region  = std::move(bs.m_region);
		}

		BatchedSprite& BatchedSprite::operator=(BatchedSprite&& bs) noexcept
		{
			if (this != &bs)
			{
				this->m_offset  = bs.m_offset;
				this->m_opacity = bs.m_opacity;
				this->m_region  = std::move(bs.m_region);
			}

			return *this;
		}

		BatchedSprite::~BatchedSprite() noexcept
		{
			m_offset = 0;
		}

		void BatchedSprite::create(const graphics::Rect<float>& region, float opacity)
		{
			m_region  = region;
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		void BatchedSprite::update_x_region(const float x) noexcept
		{
			m_region.m_x = x;
		}

		void BatchedSprite::update_y_region(const float y) noexcept
		{
			m_region.m_y = y;
		}

		void BatchedSprite::update_w_region(const float w) noexcept
		{
			m_region.m_width = w;
		}

		void BatchedSprite::update_h_region(const float h) noexcept
		{
			m_region.m_height = h;
		}

		void BatchedSprite::update_region(const float x, const float y, const float w, const float h) noexcept
		{
			m_region.m_x      = x;
			m_region.m_y      = y;
			m_region.m_width  = w;
			m_region.m_height = h;
		}

		void BatchedSprite::update_region(const graphics::Rect<float>& quad) noexcept
		{
			update_region(quad.m_x, quad.m_y, quad.m_width, quad.m_height);
		}

		void BatchedSprite::set_opacity(const float opacity) noexcept
		{
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		const float BatchedSprite::opacity() const noexcept
		{
			return m_opacity;
		}

		const int BatchedSprite::get_width() const noexcept
		{
			return m_region.m_width;
		}

		const int BatchedSprite::get_height() const noexcept
		{
			return m_region.m_height;
		}

		const graphics::Rect<float>& BatchedSprite::get_region() const noexcept
		{
			return m_region;
		}
	} // namespace components
} // namespace galaxy