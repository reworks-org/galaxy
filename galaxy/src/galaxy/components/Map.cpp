///
/// Map.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"

#include "Map.hpp"

namespace galaxy
{
	namespace components
	{
		Map::Map()
			: Renderable {}
		{
			m_spritebatch.init(10000);
		}

		Map::Map(const int total_tiles)
		{
			m_spritebatch.init(total_tiles);
		}

		Map::Map(Map&& m)
			: Renderable {std::move(m)}
		{
			this->m_animations  = std::move(m.m_animations);
			this->m_spritebatch = std::move(m.m_spritebatch);
		}

		Map& Map::operator=(Map&& m)
		{
			if (this != &m)
			{
				this->Renderable::operator=(std::move(m));

				this->m_animations  = std::move(m.m_animations);
				this->m_spritebatch = std::move(m.m_spritebatch);
			}

			return *this;
		}

		Map::~Map()
		{
			m_animations.clear();
			m_spritebatch.flush();
		}

		void Map::configure(const unsigned int texture_id, const int layer)
		{
			m_layer          = layer;
			m_texture_handle = texture_id;
		}

		const graphics::VertexArray& Map::get_vao() const
		{
			return m_spritebatch.vao();
		}
	} // namespace components
} // namespace galaxy