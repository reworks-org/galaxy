///
/// BatchSprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/TextureBook.hpp"

#include "BatchSprite.hpp"

namespace galaxy
{
	namespace components
	{
		BatchSprite::BatchSprite() noexcept
		    : Serializable {this}, m_key {""}, m_index {0}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_clip {0.0f, 0.0f}, m_depth {0}, m_opacity {255}
		{
		}

		BatchSprite::BatchSprite(const nlohmann::json& json)
		    : Serializable {this}, m_key {""}, m_index {0}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_clip {0.0f, 0.0f}, m_depth {0}, m_opacity {255}
		{
			deserialize(json);
		}

		BatchSprite::BatchSprite(BatchSprite&& bs) noexcept
		    : Serializable {this}
		{
			this->m_clip    = std::move(bs.m_clip);
			this->m_key     = std::move(bs.m_key);
			this->m_region  = std::move(bs.m_region);
			this->m_index   = bs.m_index;
			this->m_depth   = bs.m_depth;
			this->m_opacity = bs.m_opacity;
		}

		BatchSprite& BatchSprite::operator=(BatchSprite&& bs) noexcept
		{
			if (this != &bs)
			{
				this->m_clip    = std::move(bs.m_clip);
				this->m_key     = std::move(bs.m_key);
				this->m_region  = std::move(bs.m_region);
				this->m_index   = bs.m_index;
				this->m_depth   = bs.m_depth;
				this->m_opacity = bs.m_opacity;
			}

			return *this;
		}

		BatchSprite::~BatchSprite() noexcept
		{
		}

		void BatchSprite::create(const math::Rect<float>& region, const int depth, unsigned int index) noexcept
		{
			m_region = region;
			m_index  = index;

			m_clip = {0.0f, 0.0f};
			set_depth(depth);
		}

		void BatchSprite::create(std::string_view texture_key, const int depth) noexcept
		{
			auto info = SL_HANDLE.texturebook()->search(texture_key);
			if (info != std::nullopt)
			{
				m_key    = static_cast<std::string>(texture_key);
				m_region = info.value().m_region;
				m_index  = info.value().m_index;

				m_clip = {0.0f, 0.0f};
				set_depth(depth);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to get texture from textureatlas for batchsprite: {0}.", texture_key);
			}
		}

		void BatchSprite::update_region(std::string_view texture_key) noexcept
		{
			create(texture_key, m_depth);
		}

		void BatchSprite::set_depth(const int depth) noexcept
		{
			m_depth = std::clamp(depth, 0, 1000);
		}

		void BatchSprite::set_opacity(const std::uint8_t opacity) noexcept
		{
			m_opacity = std::clamp<std::uint8_t>(opacity, 0, 255);
		}

		void BatchSprite::clip_width(const float width) noexcept
		{
			m_clip.x         = width;
			m_region.m_width = width;
		}

		void BatchSprite::clip_height(const float height) noexcept
		{
			m_clip.y          = height;
			m_region.m_height = height;
		}

		const glm::vec2& BatchSprite::get_clip() const noexcept
		{
			return m_clip;
		}

		const int BatchSprite::get_depth() const noexcept
		{
			return m_depth;
		}

		const std::uint8_t BatchSprite::get_opacity() const noexcept
		{
			return m_opacity;
		}

		const math::Rect<float>& BatchSprite::get_region() const noexcept
		{
			return m_region;
		}

		const std::string& BatchSprite::get_key() const noexcept
		{
			return m_key;
		}

		const unsigned int BatchSprite::get_atlas_index() const noexcept
		{
			return m_index;
		}

		nlohmann::json BatchSprite::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["texture-key"] = m_key;
			json["depth"]       = m_depth;
			json["opacity"]     = m_opacity;

			json["clip"]      = nlohmann::json::object();
			json["clip"]["w"] = m_clip.x;
			json["clip"]["h"] = m_clip.y;

			json["region"]      = nlohmann::json::object();
			json["region"]["x"] = m_region.m_x;
			json["region"]["y"] = m_region.m_y;
			json["region"]["w"] = m_region.m_width;
			json["region"]["h"] = m_region.m_height;

			return json;
		}

		void BatchSprite::deserialize(const nlohmann::json& json)
		{
			if (json.count("texture-key") > 0)
			{
				create(json.at("texture-key"), json.at("depth"));
			}
			else if (json.count("region") > 0)
			{
				const auto& region = json.at("region");
				m_region.m_x       = region.at("x");
				m_region.m_y       = region.at("y");
				m_region.m_width   = region.at("w");
				m_region.m_height  = region.at("h");

				create(region, json.at("depth"));
			}

			set_opacity(json.at("opacity"));

			if (json.count("clip") > 0)
			{
				const auto& clip = json.at("clip");
				m_clip.x         = clip.at("w");
				m_clip.y         = clip.at("h");
			}
		}
	} // namespace components
} // namespace galaxy