///
/// MapUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/algorithm/Algorithm.hpp"
#include "galaxy/error/Log.hpp"

#include "MapUtils.hpp"

namespace galaxy
{
	namespace map
	{
		graphics::Colour map::parse_hex_colour(std::string_view hex_colour)
		{
			graphics::Colour colour;
			auto str = static_cast<std::string>(hex_colour);

			if (str[0] == '#')
			{
				str.erase(0, 1);
			}

			if (str.length() == 8)
			{
				if (std::sscanf(str.c_str(), "%02hhx%02hhx%02hhx%02hhx", &colour.m_alpha, &colour.m_red, &colour.m_green, &colour.m_blue) != 4)
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to fill 4 arguments for std::sscanf.");
				}
			}
			else if (str.length() == 6)
			{
				if (std::sscanf(str.c_str(), "%02hhx%02hhx%02hhx", &colour.m_red, &colour.m_green, &colour.m_blue) != 3)
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to fill 3 arguments for std::sscanf.");
				}

				colour.m_alpha = 255;
			}

			return colour;
		}

		std::tuple<bool, bool, bool> get_tile_flags(const int gid) noexcept
		{
			const bool flip_hori = !!(gid & FLIPPED_HORIZONTALLY_FLAG);
			const bool flip_vert = !!(gid & FLIPPED_VERTICALLY_FLAG);
			const bool flip_diag = !!(gid & FLIPPED_DIAGONALLY_FLAG);

			return std::make_tuple(flip_hori, flip_vert, flip_diag);
		}

		const int unset_tile_flags(const int gid) noexcept
		{
			const int flags = ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
			return gid & flags;
		}
	} // namespace map
} // namespace galaxy