///
/// BatchedSprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/res/TextureAtlas.hpp"

#include "BatchedSprite.hpp"

namespace galaxy
{
	namespace components
	{
		BatchedSprite::BatchedSprite() noexcept
		    : m_opacity {1.0f}, m_info {nullptr}, m_offset {0}, m_z_level {0}
		{
		}

		BatchedSprite::BatchedSprite(const nlohmann::json& json)
		    : m_opacity {1.0f}, m_info {nullptr}, m_offset {0}, m_z_level {0}
		{
			create(json.at("texture-atlas-id"), json.at("opacity"));
		}

		BatchedSprite::BatchedSprite(BatchedSprite&& bs) noexcept
		{
			this->m_offset  = bs.m_offset;
			this->m_opacity = bs.m_opacity;
			this->m_info    = bs.m_info;
		}

		BatchedSprite& BatchedSprite::operator=(BatchedSprite&& bs) noexcept
		{
			if (this != &bs)
			{
				this->m_offset  = bs.m_offset;
				this->m_opacity = bs.m_opacity;
				this->m_info    = bs.m_info;
			}

			return *this;
		}

		BatchedSprite::~BatchedSprite() noexcept
		{
			m_offset = 0;
			m_info   = nullptr;
		}

		void BatchedSprite::create(std::string_view texture_atlas_id, float opacity)
		{
			m_info    = &SL_HANDLE.atlas()->get_tex_info(texture_atlas_id);
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		void BatchedSprite::update(std::string_view id)
		{
			m_info = &SL_HANDLE.atlas()->get_tex_info(id);
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
			return m_info->m_region.m_width;
		}

		const int BatchedSprite::get_height() const noexcept
		{
			return m_info->m_region.m_height;
		}

		graphics::TextureInfo* const BatchedSprite::tex_info() const noexcept
		{
			return m_info;
		}
	} // namespace components
} // namespace galaxy