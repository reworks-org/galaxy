///
/// Sprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/TextureAtlas.hpp"

#include "Sprite.hpp"

namespace galaxy
{
	namespace components
	{
		Sprite::Sprite()
			: Serializable {}
			, m_opacity {1.0f}
			, m_width {0.0f}
			, m_height {0.0f}
		{
		}

		Sprite::Sprite(const nlohmann::json& json)
			: Serializable {}
			, m_opacity {1.0f}
			, m_width {0.0f}
			, m_height {0.0f}
		{
			deserialize(json);
		}

		Sprite::Sprite(Sprite* ptr)
			: Serializable {}
			, m_opacity {1.0f}
			, m_width {0.0f}
			, m_height {0.0f}
		{
			create(ptr->m_tex_name, ptr->m_layer, ptr->m_opacity);
		}

		Sprite::Sprite(Sprite&& s)
			: Renderable {std::move(s)}
			, Serializable {}
			, m_opacity {1.0f}
			, m_width {0.0f}
			, m_height {0.0f}
		{
			this->m_vao      = std::move(s.m_vao);
			this->m_tex_name = std::move(s.m_tex_name);
			this->m_opacity  = s.m_opacity;
			this->m_width    = s.m_width;
			this->m_height   = s.m_height;
		}

		Sprite& Sprite::operator=(Sprite&& s)
		{
			if (this != &s)
			{
				this->Renderable::operator=(std::move(s));

				this->m_vao      = std::move(s.m_vao);
				this->m_tex_name = std::move(s.m_tex_name);
				this->m_opacity  = s.m_opacity;
				this->m_width    = s.m_width;
				this->m_height   = s.m_height;
			}

			return *this;
		}

		Sprite::~Sprite()
		{
		}

		void Sprite::create(const std::string& texture, const int layer, const float opacity)
		{
			auto& atlas = core::ServiceLocator<resource::TextureAtlas>::ref();

			const auto info_opt = atlas.query(texture);
			if (info_opt.has_value())
			{
				const auto& info = info_opt.value().get();

				m_tex_name       = texture;
				m_opacity        = std::clamp(opacity, 0.0f, 1.0f);
				m_width          = static_cast<float>(info.m_region.m_width);
				m_height         = static_cast<float>(info.m_region.m_height);
				m_layer          = layer;
				m_texture_handle = info.m_handle;

				std::array<graphics::Vertex, 4> vertices;

				vertices[0].m_pos    = {0.0f, 0.0f};
				vertices[0].m_texels = info.m_texel_region.ul_texels();

				vertices[1].m_pos    = {m_width, 0.0f};
				vertices[1].m_texels = info.m_texel_region.ur_texels();

				vertices[2].m_pos    = {m_width, m_height};
				vertices[2].m_texels = info.m_texel_region.br_texels();

				vertices[3].m_pos    = {0.0f, m_height};
				vertices[3].m_texels = info.m_texel_region.bl_texels();

				m_vao.create(vertices, graphics::StorageFlag::STATIC_DRAW, graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to query texture atlas for '{0}'.", texture);
			}
		}

		void Sprite::create(const std::string& texture, const graphics::iRect& texture_rect, const int layer, const float opacity)
		{
			auto& atlas = core::ServiceLocator<resource::TextureAtlas>::ref();

			const auto info_opt = atlas.query(texture);
			if (info_opt.has_value())
			{
				const auto& info = info_opt.value().get();

				m_tex_name       = texture;
				m_opacity        = std::clamp(opacity, 0.0f, 1.0f);
				m_width          = static_cast<float>(info.m_region.m_width);
				m_height         = static_cast<float>(info.m_region.m_height);
				m_layer          = layer;
				m_texture_handle = info.m_handle;

				std::array<graphics::Vertex, 4> vertices;

				const auto off_x = info.m_region.m_x + texture_rect.m_x;
				const auto off_y = info.m_region.m_y + texture_rect.m_y;

				vertices[0].m_pos      = {0.0f, 0.0f};
				vertices[0].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
				vertices[0].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

				vertices[1].m_pos      = {m_width, 0.0f};
				vertices[1].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + texture_rect.m_width, info.m_sheet_width);
				vertices[1].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

				vertices[2].m_pos      = {m_width, m_height};
				vertices[2].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + texture_rect.m_width, info.m_sheet_width);
				vertices[2].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + texture_rect.m_height, info.m_sheet_height);

				vertices[3].m_pos      = {0.0f, m_height};
				vertices[3].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
				vertices[3].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + texture_rect.m_height, info.m_sheet_height);

				m_vao.create(vertices, graphics::StorageFlag::STATIC_DRAW, graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);
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

				m_tex_name       = texture;
				m_width          = static_cast<float>(info.m_region.m_width);
				m_height         = static_cast<float>(info.m_region.m_height);
				m_texture_handle = info.m_handle;

				std::array<graphics::Vertex, 4> vertices;

				vertices[0].m_pos    = {0.0f, 0.0f};
				vertices[0].m_texels = info.m_texel_region.ul_texels();

				vertices[1].m_pos    = {m_width, 0.0f};
				vertices[1].m_texels = info.m_texel_region.ur_texels();

				vertices[2].m_pos    = {m_width, m_height};
				vertices[2].m_texels = info.m_texel_region.br_texels();

				vertices[3].m_pos    = {0.0f, m_height};
				vertices[3].m_texels = info.m_texel_region.bl_texels();

				m_vao.sub_buffer(0, vertices);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to query texture atlas for '{0}'.", texture);
			}
		}

		void Sprite::update(const std::string& texture, const graphics::iRect& texture_rect)
		{
			auto& atlas = core::ServiceLocator<resource::TextureAtlas>::ref();

			const auto info_opt = atlas.query(texture);
			if (info_opt.has_value())
			{
				const auto& info = info_opt.value().get();

				m_tex_name       = texture;
				m_width          = static_cast<float>(info.m_region.m_width);
				m_height         = static_cast<float>(info.m_region.m_height);
				m_texture_handle = info.m_handle;

				std::array<graphics::Vertex, 4> vertices;

				const auto off_x = info.m_region.m_x + texture_rect.m_x;
				const auto off_y = info.m_region.m_y + texture_rect.m_y;

				vertices[0].m_pos      = {0.0f, 0.0f};
				vertices[0].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
				vertices[0].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

				vertices[1].m_pos      = {m_width, 0.0f};
				vertices[1].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + texture_rect.m_width, info.m_sheet_width);
				vertices[1].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

				vertices[2].m_pos      = {m_width, m_height};
				vertices[2].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + texture_rect.m_width, info.m_sheet_width);
				vertices[2].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + texture_rect.m_height, info.m_sheet_height);

				vertices[3].m_pos      = {0.0f, m_height};
				vertices[3].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
				vertices[3].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + texture_rect.m_height, info.m_sheet_height);

				m_vao.sub_buffer(0, vertices);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to query texture atlas for '{0}'.", texture);
			}
		}

		void Sprite::set_opacity(const float opacity)
		{
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		float Sprite::get_opacity() const
		{
			return m_opacity;
		}

		float Sprite::get_width() const
		{
			return m_width;
		}

		float Sprite::get_height() const
		{
			return m_height;
		}

		const std::string& Sprite::get_texture_name() const
		{
			return m_tex_name;
		}

		const graphics::VertexArray& Sprite::get_vao() const
		{
			return m_vao;
		}

		nlohmann::json Sprite::serialize()
		{
			nlohmann::json json  = "{}"_json;
			json["texture_name"] = m_tex_name;
			json["opacity"]      = m_opacity;
			json["layer"]        = m_layer;

			return json;
		}

		void Sprite::deserialize(const nlohmann::json& json)
		{
			create(json.at("texture_name"), json.at("layer"), json.at("opacity"));
		}
	} // namespace components
} // namespace galaxy
