///
/// Batch.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Batch.hpp"

namespace galaxy
{
	namespace components
	{
		Batch::Batch() noexcept
			: Renderable {}
			, m_opacity {0.0f}
			, m_width {0.0f}
			, m_height {0.0f}
		{
		}

		Batch::Batch(std::string_view Batch) noexcept
			: Renderable {}
			, m_opacity {0.0f}
			, m_width {0.0f}
			, m_height {0.0f}
		{
		}

		Batch::Batch(Batch&& t) noexcept
			: Renderable {std::move(t)}
			, m_opacity {0.0f}
			, m_width {0.0f}
			, m_height {0.0f}
		{
			this->m_vao     = std::move(t.m_vao);
			this->m_opacity = t.m_opacity;
			this->m_width   = t.m_width;
			this->m_height  = t.m_height;
		}

		Batch& Batch::operator=(Batch&& t) noexcept
		{
			if (this != &t)
			{
				this->m_vao     = std::move(t.m_vao);
				this->m_opacity = t.m_opacity;
				this->m_width   = t.m_width;
				this->m_height  = t.m_height;
			}

			return *this;
		}

		Batch::~Batch() noexcept
		{
		}

		void Batch::create(resource::TextureAtlas::Info& info, std::span<graphics::Vertex> vertices, const int layer, const float opacity)
		{
			m_width  = static_cast<float>(info.m_region.m_width);
			m_height = static_cast<float>(info.m_region.m_height);

			set_layer(layer);
			set_opacity(opacity);
			set_texture(info.m_handle);

			const auto quad_count = static_cast<int>(std::trunc(static_cast<float>(vertices.size()) / 4.0f));

			std::vector<unsigned int> indices;
			indices.reserve(quad_count * 6);

			auto increment = 0;
			for (auto counter = 0; counter < quad_count; counter++)
			{
				indices.push_back(0 + increment);
				indices.push_back(1 + increment);
				indices.push_back(3 + increment);
				indices.push_back(1 + increment);
				indices.push_back(2 + increment);
				indices.push_back(3 + increment);

				increment += 4;
			}

			m_vao.create(vertices, graphics::StorageFlag::DYNAMIC_DRAW, indices, graphics::StorageFlag::DYNAMIC_DRAW);

			configure();
		}

		void Batch::set_opacity(const float opacity) noexcept
		{
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		float Batch::get_opacity() const noexcept
		{
			return m_opacity;
		}

		float Batch::get_width() const noexcept
		{
			return m_width;
		}

		float Batch::get_height() const noexcept
		{
			return m_height;
		}

		void Batch::configure() noexcept
		{
			set_vao(m_vao.id());
			set_index_count(m_vao.index_count());
			set_instance_count(1);
			set_primitive_type(graphics::Primitives::TRIANGLE);
		}
	} // namespace components
} // namespace galaxy