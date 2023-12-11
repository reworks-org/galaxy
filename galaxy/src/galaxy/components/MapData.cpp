///
/// MapData.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"
#include "galaxy/graphics/Primitives.hpp"

#include "MapData.hpp"

namespace galaxy
{
	namespace components
	{
		MapData::MapData()
			: m_texture {0}
			, m_layer {0}
		{
		}

		MapData::MapData(const int total_tiles)
			: m_texture {0}
			, m_layer {0}
		{
			m_spritebatch.init(total_tiles);
		}

		MapData::MapData(MapData&& m)
		{
			this->m_animations  = std::move(m.m_animations);
			this->m_spritebatch = std::move(m.m_spritebatch);
			this->m_layer       = m.m_layer;
			this->m_texture     = m.m_texture;

			m.m_texture = 0;
		}

		MapData& MapData::operator=(MapData&& m)
		{
			if (this != &m)
			{
				this->m_animations  = std::move(m.m_animations);
				this->m_spritebatch = std::move(m.m_spritebatch);
				this->m_layer       = m.m_layer;
				this->m_texture     = m.m_texture;

				m.m_texture = 0;
			}

			return *this;
		}

		MapData::~MapData()
		{
			m_texture = 0;

			m_animations.clear();
			m_spritebatch.flush();
		}

		void MapData::set_texture(const unsigned int texture, const int layer)
		{
			m_texture = texture;
			m_layer   = layer;
		}

		int MapData::get_instances() const
		{
			return 1;
		}

		unsigned int MapData::get_mode() const
		{
			return graphics::primitive_to_gl(graphics::Primitives::TRIANGLE);
		}

		unsigned int MapData::get_vao() const
		{
			return m_spritebatch.vao().id();
		}

		unsigned int MapData::get_texture() const
		{
			return m_texture;
		}

		unsigned int MapData::get_count() const
		{
			return m_spritebatch.count();
		}

		int MapData::get_layer() const
		{
			return m_layer;
		}
	} // namespace components
} // namespace galaxy
