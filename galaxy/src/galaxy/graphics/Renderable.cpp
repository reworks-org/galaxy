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
		{
		}

		Renderable::~Renderable() noexcept
		{
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