///
/// TileAnimation.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "TileAnimation.hpp"

namespace galaxy
{
	namespace map
	{
		TileAnimation::TileAnimation()
			: m_anim {nullptr}
			, m_tileset {nullptr}

		{
		}

		TileAnimation::TileAnimation(TileAnimation&& a)
			: m_anim {nullptr}
			, m_tileset {nullptr}
		{
			this->m_anim             = a.m_anim;
			this->m_tileset          = a.m_tileset;
			this->m_tiles_to_animate = std::move(a.m_tiles_to_animate);

			a.m_anim    = nullptr;
			a.m_tileset = nullptr;
		}

		TileAnimation& TileAnimation::operator=(TileAnimation&& a)
		{
			if (this != &a)
			{
				this->m_anim             = a.m_anim;
				this->m_tileset          = a.m_tileset;
				this->m_tiles_to_animate = std::move(a.m_tiles_to_animate);

				a.m_anim    = nullptr;
				a.m_tileset = nullptr;
			}

			return *this;
		}

		TileAnimation::~TileAnimation()
		{
			m_anim    = nullptr;
			m_tileset = nullptr;
		}
	} // namespace map
} // namespace galaxy
