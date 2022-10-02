///
/// Renderable.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Renderable.hpp"

namespace galaxy
{
	namespace graphics
	{
		Renderable::Renderable() noexcept
			: m_vao_id {0}
			, m_texture_id {0}
			, m_index_count {0}
			, m_instances {1}
			, m_type {0}
			, m_layer {0}
			, m_shader_sort_id {0}
		{
		}

		Renderable::Renderable(Renderable&& r) noexcept
		{
			// Dont release existing memory here, as we dont own it.

			this->m_vao_id           = r.m_vao_id;
			this->m_texture_id       = r.m_texture_id;
			this->m_index_count      = r.m_index_count;
			this->m_instances        = r.m_instances;
			this->m_type             = r.m_type;
			this->m_layer            = r.m_layer;
			this->m_configure_shader = std::move(r.m_configure_shader);
			this->m_shader_sort_id   = r.m_shader_sort_id;

			r.m_vao_id         = 0;
			r.m_texture_id     = 0;
			r.m_shader_sort_id = 0;
		}

		Renderable& Renderable::operator=(Renderable&& r) noexcept
		{
			if (this != &r)
			{
				// Dont release existing memory here, as we dont own it.

				this->m_vao_id           = r.m_vao_id;
				this->m_texture_id       = r.m_texture_id;
				this->m_index_count      = r.m_index_count;
				this->m_instances        = r.m_instances;
				this->m_type             = r.m_type;
				this->m_layer            = r.m_layer;
				this->m_configure_shader = std::move(r.m_configure_shader);
				this->m_shader_sort_id   = r.m_shader_sort_id;

				r.m_vao_id         = 0;
				r.m_texture_id     = 0;
				r.m_shader_sort_id = 0;
			}

			return *this;
		}

		Renderable::~Renderable() noexcept
		{
		}

		void Renderable::set_primitive_type(const Primitives type) noexcept
		{
			m_type = static_cast<unsigned int>(type);
		}
	} // namespace graphics
} // namespace galaxy