///
/// TileMap.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/Textures.hpp"

#include "TileMap.hpp"

namespace galaxy
{
	namespace components
	{
		TileMap::TileMap()
			: m_texture {nullptr}
		{
		}

		TileMap::TileMap(TileMap&& t)
		{
			this->m_texture = t.m_texture;
			this->m_batch   = std::move(t.m_batch);
		}

		TileMap& TileMap::operator=(TileMap&& t)
		{
			if (this != &t)
			{
				this->m_texture = t.m_texture;
				this->m_batch   = std::move(t.m_batch);
			}

			return *this;
		}

		TileMap::~TileMap()
		{
			m_texture = nullptr;
		}

		void TileMap::set_texture(const std::string& texture)
		{
			auto& cache = core::ServiceLocator<resource::Textures>::ref();
			auto  tex   = cache.get(texture);

			if (tex)
			{
				m_texture = tex;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to query texture storage for '{0}'.", texture);
			}
		}

		graphics::Texture2D* TileMap::get_texture()
		{
			return m_texture;
		}
	} // namespace components
} // namespace galaxy
