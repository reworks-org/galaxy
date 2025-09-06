///
/// RectPack.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "RectPack.hpp"

namespace galaxy
{
	RectPack::RectPack() noexcept
		: m_width {0}
		, m_height {0}
	{
	}

	RectPack::~RectPack() noexcept
	{
		m_free_rects.clear();
	}

	void RectPack::init(const int width, const int height) noexcept
	{
		m_width  = width;
		m_height = height;

		m_free_rects.emplace_back(0, 0, m_width, m_height);
	}

	std::optional<iRect> RectPack::pack(const int width, const int height)
	{
		// Result.
		std::optional<iRect> result = std::nullopt;

		// Go over each space in the rectangle, in reverse order (i.e. smallest -> largest).
		for (auto rit = m_free_rects.rbegin(); rit != m_free_rects.rend();)
		{
			auto& space = *rit;

			// Check if rect can fit into space.
			if (width <= space.m_width && height <= space.m_height)
			{
				// Make the packed area rectangle.
				result = std::make_optional<iRect>(space.m_xpos, space.m_ypos, width, height);

				// Check to see if shape fills completely.
				if (width == space.m_width && height == space.m_height)
				{
					// Destroy since not free space anymore.
					std::advance(rit, 1);
					m_free_rects.erase(rit.base());
				}
				else if (width == space.m_width)
				{
					// If just width fits, shrink new space.
					space.m_ypos   += height;
					space.m_height -= height;
				}
				else if (height == space.m_height)
				{
					// Same as width, for height.
					space.m_xpos  += width;
					space.m_width -= width;
				}
				else
				{
					// Otherwise, split up existing space.
					iRect temp = {space.m_xpos + width, space.m_ypos, space.m_width - width, height};

					space.m_ypos   += height;
					space.m_height -= height;

					m_free_rects.emplace_back(temp);
				}

				// If it can fit, no longer need to keep iterating through.
				break;
			}
			else
			{
				// Try next space.
				++rit;
			}
		}

		return result;
	}

	void RectPack::clear() noexcept
	{
		m_free_rects.clear();
		m_free_rects.emplace_back(0, 0, m_width, m_height);
	}

	int RectPack::get_width() const noexcept
	{
		return m_width;
	}

	int RectPack::get_height() const noexcept
	{
		return m_height;
	}

	const std::vector<iRect>& RectPack::get_free_space() const noexcept
	{
		return m_free_rects;
	}
} // namespace galaxy
