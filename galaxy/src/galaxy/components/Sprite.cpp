///
/// Sprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Primitives.hpp"
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
			, m_tex_id {0}
			, m_layer {0}
		{
		}

		Sprite::Sprite(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Sprite::Sprite(Sprite* ptr)
			: Serializable {}
			, m_opacity {1.0f}
			, m_width {0.0f}
			, m_height {0.0f}
			, m_tex_id {0}
			, m_layer {0}
		{
			create(ptr->m_tex_name, ptr->m_layer, ptr->m_opacity);
		}

		Sprite::Sprite(Sprite&& s)
			: Serializable {}
		{
			this->m_vao      = std::move(s.m_vao);
			this->m_tex_name = std::move(s.m_tex_name);
			this->m_opacity  = s.m_opacity;
			this->m_width    = s.m_width;
			this->m_height   = s.m_height;
			this->m_tex_id   = s.m_tex_id;
			this->m_layer    = s.m_layer;

			s.m_tex_id = 0;
		}

		Sprite& Sprite::operator=(Sprite&& s)
		{
			if (this != &s)
			{
				this->m_vao      = std::move(s.m_vao);
				this->m_tex_name = std::move(s.m_tex_name);
				this->m_opacity  = s.m_opacity;
				this->m_width    = s.m_width;
				this->m_height   = s.m_height;
				this->m_tex_id   = s.m_tex_id;
				this->m_layer    = s.m_layer;

				s.m_tex_id = 0;
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

				m_tex_name = texture;
				m_opacity  = std::clamp(opacity, 0.0f, 1.0f);
				m_width    = static_cast<float>(info.m_region.width);
				m_height   = static_cast<float>(info.m_region.height);
				m_layer    = layer;
				m_tex_id   = info.m_handle;

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

		void Sprite::create(const std::string& texture, const math::iRect& texture_rect, const int layer, const float opacity)
		{
			auto& atlas = core::ServiceLocator<resource::TextureAtlas>::ref();

			const auto info_opt = atlas.query(texture);
			if (info_opt.has_value())
			{
				const auto& info = info_opt.value().get();

				m_tex_name = texture;
				m_opacity  = std::clamp(opacity, 0.0f, 1.0f);
				m_width    = static_cast<float>(info.m_region.width);
				m_height   = static_cast<float>(info.m_region.height);
				m_layer    = layer;
				m_tex_id   = info.m_handle;

				std::array<graphics::Vertex, 4> vertices;

				const auto off_x = info.m_region.x + texture_rect.x;
				const auto off_y = info.m_region.y + texture_rect.y;

				vertices[0].m_pos      = {0.0f, 0.0f};
				vertices[0].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
				vertices[0].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

				vertices[1].m_pos      = {m_width, 0.0f};
				vertices[1].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + texture_rect.width, info.m_sheet_width);
				vertices[1].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

				vertices[2].m_pos      = {m_width, m_height};
				vertices[2].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + texture_rect.width, info.m_sheet_width);
				vertices[2].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + texture_rect.height, info.m_sheet_height);

				vertices[3].m_pos      = {0.0f, m_height};
				vertices[3].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
				vertices[3].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + texture_rect.height, info.m_sheet_height);

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

				m_tex_name = texture;
				m_width    = static_cast<float>(info.m_region.width);
				m_height   = static_cast<float>(info.m_region.height);
				m_tex_id   = info.m_handle;

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

		void Sprite::update(const std::string& texture, const math::iRect& texture_rect)
		{
			auto& atlas = core::ServiceLocator<resource::TextureAtlas>::ref();

			const auto info_opt = atlas.query(texture);
			if (info_opt.has_value())
			{
				const auto& info = info_opt.value().get();

				m_tex_name = texture;
				m_width    = static_cast<float>(info.m_region.width);
				m_height   = static_cast<float>(info.m_region.height);
				m_tex_id   = info.m_handle;

				std::array<graphics::Vertex, 4> vertices;

				const auto off_x = info.m_region.x + texture_rect.x;
				const auto off_y = info.m_region.y + texture_rect.y;

				vertices[0].m_pos      = {0.0f, 0.0f};
				vertices[0].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
				vertices[0].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

				vertices[1].m_pos      = {m_width, 0.0f};
				vertices[1].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + texture_rect.width, info.m_sheet_width);
				vertices[1].m_texels.y = resource::TextureAtlas::map_y_texel(off_y, info.m_sheet_height);

				vertices[2].m_pos      = {m_width, m_height};
				vertices[2].m_texels.x = resource::TextureAtlas::map_x_texel(off_x + texture_rect.width, info.m_sheet_width);
				vertices[2].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + texture_rect.height, info.m_sheet_height);

				vertices[3].m_pos      = {0.0f, m_height};
				vertices[3].m_texels.x = resource::TextureAtlas::map_x_texel(off_x, info.m_sheet_width);
				vertices[3].m_texels.y = resource::TextureAtlas::map_y_texel(off_y + texture_rect.height, info.m_sheet_height);

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

		int Sprite::get_instances() const
		{
			return 1;
		}

		unsigned int Sprite::get_mode() const
		{
			return graphics::primitive_to_gl(graphics::Primitives::TRIANGLE);
		}

		unsigned int Sprite::get_vao() const
		{
			return m_vao.id();
		}

		unsigned int Sprite::get_texture() const
		{
			return m_tex_id;
		}

		unsigned int Sprite::get_count() const
		{
			return m_vao.index_count();
		}

		int Sprite::get_layer() const
		{
			return m_layer;
		}

		nlohmann::json Sprite::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["texture"]     = m_tex_name;
			json["opacity"]     = m_opacity;
			json["layer"]       = m_layer;

			return json;
		}

		void Sprite::deserialize(const nlohmann::json& json)
		{
			create(json.at("texture"), json.at("layer"), json.at("opacity"));
		}
	} // namespace components
} // namespace galaxy
