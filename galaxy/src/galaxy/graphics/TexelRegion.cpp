///
/// TexelRegion.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "TexelRegion.hpp"

namespace galaxy
{
	namespace graphics
	{
		const glm::vec2& TexelRegion::ul_texels() const noexcept
		{
			return m_ul_texels;
		}

		const glm::vec2& TexelRegion::ur_texels() const noexcept
		{
			return m_ur_texels;
		}

		const glm::vec2& TexelRegion::bl_texels() const noexcept
		{
			return m_bl_texels;
		}

		const glm::vec2& TexelRegion::br_texels() const noexcept
		{
			return m_br_texels;
		}
	} // namespace graphics
} // namespace galaxy