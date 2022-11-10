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
		Map::Map() noexcept
			: Renderable {}
		{
			m_spritebatch.init(10000);
		}

		Map::Map(const int total_tiles) noexcept
		{
			m_spritebatch.init(total_tiles);
		}

		Map::Map(Map&& m) noexcept
			: Renderable {std::move(m)}
		{
			this->m_animations  = std::move(m.m_animations);
			this->m_spritebatch = std::move(m.m_spritebatch);
		}

		Map& Map::operator=(Map&& m) noexcept
		{
			if (this != &m)
			{
				this->Renderable::operator=(std::move(m));

				this->m_animations  = std::move(m.m_animations);
				this->m_spritebatch = std::move(m.m_spritebatch);
			}

			return *this;
		}

		Map::~Map() noexcept
		{
			m_animations.clear();
			m_spritebatch.flush();
		}

		void Map::configure(const unsigned int texture_id, const int layer) noexcept
		{
			m_vao_id      = m_spritebatch.vao().id();
			m_texture_id  = texture_id;
			m_index_count = m_spritebatch.count();
			m_instances   = 1;
			m_layer       = layer;
			set_primitive_type(graphics::Primitives::TRIANGLE);
		}

		int Map::get_layer() const noexcept
		{
			return m_layer;
		}

		void Map::configure() noexcept
		{
			GALAXY_LOG(GALAXY_FATAL, "Should not be called.");
			static_assert("Map::configure() Should not be called.");
		}
	} // namespace components
} // namespace galaxy