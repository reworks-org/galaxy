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
			: m_vao {0}
			, m_texture {0}
			, m_count {0}
			, m_instances {1}
			, m_type {0}
			, m_layer {0}
			, m_shader {0}
		{
		}

		Renderable::Renderable(Renderable&& r) noexcept
		{
			this->m_vao       = r.m_vao;
			this->m_texture   = r.m_texture;
			this->m_count     = r.m_count;
			this->m_instances = r.m_instances;
			this->m_type      = r.m_type;
			this->m_layer     = r.m_layer;
			this->m_shader    = r.m_shader;
		}

		Renderable& Renderable::operator=(Renderable&& r) noexcept
		{
			if (this != &r)
			{
				this->m_vao       = r.m_vao;
				this->m_texture   = r.m_texture;
				this->m_count     = r.m_count;
				this->m_instances = r.m_instances;
				this->m_type      = r.m_type;
				this->m_layer     = r.m_layer;
				this->m_shader    = r.m_shader;
			}

			return *this;
		}

		Renderable::~Renderable() noexcept
		{
		}

		void Renderable::set_shader(const unsigned int program) noexcept
		{
			m_shader = program;
		}

		int Renderable::get_layer() const noexcept
		{
			return m_layer;
		}

		void Renderable::set_vao(const unsigned int vao) noexcept
		{
			m_vao = vao;
		}

		void Renderable::set_texture(const unsigned int texture) noexcept
		{
			m_texture = texture;
		}

		void Renderable::set_index_count(const int count) noexcept
		{
			m_count = count;
		}

		void Renderable::set_instance_count(const int count) noexcept
		{
			m_instances = count;
		}

		void Renderable::set_primitive_type(const Primitives type) noexcept
		{
			m_type = static_cast<unsigned int>(type);
		}

		void Renderable::set_layer(const int layer) noexcept
		{
			m_layer = layer;
		}
	} // namespace graphics
} // namespace galaxy