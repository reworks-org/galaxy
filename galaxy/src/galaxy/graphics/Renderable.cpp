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
		Renderable::Renderable()
			: m_texture_handle {0}
			, m_layer {0}
		{
		}

		Renderable::Renderable(Renderable&& r)
		{
			this->m_texture_handle = r.m_texture_handle;
			this->m_layer          = r.m_layer;
		}

		Renderable& Renderable::operator=(Renderable&& r)
		{
			if (this != &r)
			{
				this->m_texture_handle = r.m_texture_handle;
				this->m_layer          = r.m_layer;
			}

			return *this;
		}

		Renderable::~Renderable()
		{
		}

		unsigned int Renderable::get_texture_handle() const
		{
			return m_texture_handle;
		}

		int Renderable::get_layer() const
		{
			return m_layer;
		}
	} // namespace graphics
} // namespace galaxy