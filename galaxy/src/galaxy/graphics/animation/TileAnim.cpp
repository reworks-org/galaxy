///
/// TileAnim.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "TileAnim.hpp"

namespace galaxy
{
	namespace graphics
	{
		TileAnim::TileAnim()
			: m_anim {nullptr}
			, m_tileset {nullptr}

		{
		}

		TileAnim::TileAnim(TileAnim&& a)
			: m_anim {nullptr}
			, m_tileset {nullptr}
		{
			this->m_anim             = a.m_anim;
			this->m_tileset          = a.m_tileset;
			this->m_tiles_to_animate = std::move(a.m_tiles_to_animate);

			a.m_anim    = nullptr;
			a.m_tileset = nullptr;
		}

		TileAnim& TileAnim::operator=(TileAnim&& a)
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

		TileAnim::~TileAnim()
		{
			m_anim    = nullptr;
			m_tileset = nullptr;
		}
	} // namespace graphics
} // namespace galaxy