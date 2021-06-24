///
/// BatchSprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/TextureBook.hpp"

#include "BatchSprite.hpp"

#define ORTHO_NEAR      0
#define ORTHO_FAR_24BIT 16777215

namespace galaxy
{
	namespace components
	{
		BatchSprite::BatchSprite() noexcept
		    : Serializable {this}, m_key {""}, m_index {0}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_clip {0.0f, 0.0f}, m_depth {0}
		{
		}

		BatchSprite::BatchSprite(const nlohmann::json& json)
		    : Serializable {this}, m_key {""}, m_index {0}, m_region {0.0f, 0.0f, 0.0f, 0.0f}, m_clip {0.0f, 0.0f}, m_depth {0}
		{
			deserialize(json);
		}

		BatchSprite::BatchSprite(BatchSprite&& bs) noexcept
		    : Serializable {this}
		{
			this->m_clip   = std::move(bs.m_clip);
			this->m_key    = std::move(bs.m_key);
			this->m_region = std::move(bs.m_region);
			this->m_index  = bs.m_index;
			this->m_depth  = bs.m_depth;
		}

		BatchSprite& BatchSprite::operator=(BatchSprite&& bs) noexcept
		{
			if (this != &bs)
			{
				this->m_clip   = std::move(bs.m_clip);
				this->m_key    = std::move(bs.m_key);
				this->m_region = std::move(bs.m_region);
				this->m_index  = bs.m_index;
				this->m_depth  = bs.m_depth;
			}

			return *this;
		}

		BatchSprite::~BatchSprite() noexcept
		{
			m_vertices.clear();
		}

		void BatchSprite::create(const math::Rect<float>& region, const int depth, unsigned int index) noexcept
		{
			m_region = region;
			m_index  = index;

			m_clip  = {0.0f, 0.0f};
			m_depth = depth;
		}

		void BatchSprite::create(std::string_view texture_key, const int depth) noexcept
		{
			auto info = SL_HANDLE.texturebook()->search(texture_key);
			if (info != std::nullopt)
			{
				m_key    = static_cast<std::string>(texture_key);
				m_region = info.value().m_region;
				m_index  = info.value().m_index;

				m_clip  = {0.0f, 0.0f};
				m_depth = depth;
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
			m_depth = std::clamp(depth, ORTHO_NEAR, ORTHO_FAR_24BIT);
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

		std::vector<graphics::Vertex>& BatchSprite::get_vertices() noexcept
		{
			return m_vertices;
		}

		nlohmann::json BatchSprite::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["texture-key"] = m_key;
			json["depth"]       = m_depth;

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
			create(json.at("texture-key"), json.at("depth"));

			if (json.count("clip") > 0)
			{
				const auto& clip = json.at("clip");
				m_clip.x         = clip.at("w");
				m_clip.y         = clip.at("h");
			}

			if (json.count("region") > 0)
			{
				const auto& region = json.at("region");
				m_region.m_x       = region.at("x");
				m_region.m_y       = region.at("y");
				m_region.m_width   = region.at("w");
				m_region.m_height  = region.at("h");
			}
		}
	} // namespace components
} // namespace galaxy