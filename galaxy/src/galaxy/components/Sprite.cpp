///
/// Sprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Sprite.hpp"

namespace galaxy
{
	namespace components
	{
		Sprite::Sprite() noexcept
			: Renderable {}
			, Serializable {}
			, m_opacity {0.0f}
			, m_width {0.0f}
			, m_height {0.0f}
		{
		}

		Sprite::Sprite(const nlohmann::json& json)
			: Renderable {}
			, Serializable {}
			, m_opacity {0.0f}
			, m_width {0.0f}
			, m_height {0.0f}
		{
			deserialize(json);
		}

		Sprite::Sprite(Sprite&& s) noexcept
			: Renderable {std::move(s)}
			, Serializable {}
			, m_opacity {0.0f}
			, m_width {0.0f}
			, m_height {0.0f}
		{
			this->m_vao     = std::move(s.m_vao);
			this->m_texture = std::move(s.m_texture);
			this->m_opacity = s.m_opacity;
			this->m_width   = s.m_width;
			this->m_height  = s.m_height;
		}

		Sprite& Sprite::operator=(Sprite&& s) noexcept
		{
			if (this != &s)
			{
				this->Renderable::operator=(std::move(s));

				this->m_vao     = std::move(s.m_vao);
				this->m_texture = std::move(s.m_texture);
				this->m_opacity = s.m_opacity;
				this->m_width   = s.m_width;
				this->m_height  = s.m_height;
			}

			return *this;
		}

		Sprite::~Sprite() noexcept
		{
		}

		void Sprite::create(const std::string& texture, const int layer, const float opacity)
		{
			auto& atlas = core::ServiceLocator<resource::TextureAtlas>::ref();

			const auto info_opt = atlas.query(texture);
			if (info_opt.has_value())
			{
				const auto& info = info_opt.value().get();

				m_width  = static_cast<float>(info.m_region.m_width);
				m_height = static_cast<float>(info.m_region.m_height);

				set_layer(layer);
				set_opacity(opacity);
				set_texture(info.m_handle);

				std::array<graphics::Vertex, 4> vertices;

				vertices[0].m_pos    = {0.0f, 0.0f};
				vertices[0].m_texels = info.m_texel_region.ul_texels();

				vertices[1].m_pos    = {m_width, 0.0f};
				vertices[1].m_texels = info.m_texel_region.ur_texels();

				vertices[2].m_pos    = {m_width, m_height};
				vertices[2].m_texels = info.m_texel_region.br_texels();

				vertices[3].m_pos    = {0.0f, m_height};
				vertices[3].m_texels = info.m_texel_region.ur_texels();

				m_vao.create(vertices, graphics::StorageFlag::STATIC_DRAW, graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);

				configure();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to query texture atlas for '{0}'.", texture);
			}
		}

		void Sprite::update(const std::string& texture)
		{
			auto& atlas = core::ServiceLocator<resource::TextureAtlas>::ref();

			const auto info_opt = atlas.query(texture);
			if (info_opt.has_value())
			{
				const auto& info = info_opt.value().get();

				m_width  = static_cast<float>(info.m_region.m_width);
				m_height = static_cast<float>(info.m_region.m_height);

				set_texture(info.m_handle);

				std::array<graphics::Vertex, 4> vertices;

				vertices[0].m_pos    = {0.0f, 0.0f};
				vertices[0].m_texels = info.m_texel_region.ul_texels();

				vertices[1].m_pos    = {m_width, 0.0f};
				vertices[1].m_texels = info.m_texel_region.ur_texels();

				vertices[2].m_pos    = {m_width, m_height};
				vertices[2].m_texels = info.m_texel_region.br_texels();

				vertices[3].m_pos    = {0.0f, m_height};
				vertices[3].m_texels = info.m_texel_region.ur_texels();

				m_vao.sub_buffer(0, vertices);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to query texture atlas for '{0}'.", texture);
			}
		}

		void Sprite::set_opacity(const float opacity) noexcept
		{
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		float Sprite::get_opacity() const noexcept
		{
			return m_opacity;
		}

		float Sprite::get_width() const noexcept
		{
			return m_width;
		}

		float Sprite::get_height() const noexcept
		{
			return m_height;
		}

		void Sprite::configure() noexcept
		{
			set_vao(m_vao.id());
			set_index_count(m_vao.index_count());
			set_instance_count(1);
			set_primitive_type(graphics::Primitives::TRIANGLE);
		}

		nlohmann::json Sprite::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["texture"]     = m_texture;
			json["opacity"]     = m_opacity;
			json["layer"]       = get_layer();

			return json;
		}

		void Sprite::deserialize(const nlohmann::json& json)
		{
			create(json.at("texture"), json.at("layer"), json.at("opacity"));
		}
	} // namespace components
} // namespace galaxy