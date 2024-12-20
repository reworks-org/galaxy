///
/// RectPack.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "RectPack.hpp"

namespace galaxy
{
	namespace math
	{
		RectPack::RectPack()
			: m_width {0}
			, m_height {0}
		{
		}

		RectPack::~RectPack()
		{
			m_free_rects.clear();
		}

		void RectPack::init(const int width, const int height)
		{
			m_width  = width;
			m_height = height;

			m_free_rects.emplace_back(0, 0, m_width, m_height);
		}

		std::optional<graphics::Rectangle> RectPack::pack(const int width, const int height)
		{
			// Result.
			std::optional<graphics::Rectangle> result = std::nullopt;

			// Go over each space in the rectangle, in reverse order (i.e. smallest -> largest).
			for (auto rit = m_free_rects.rbegin(); rit != m_free_rects.rend();)
			{
				auto& space = *rit;

				// Check if rect can fit into space.
				if (width <= space.width && height <= space.height)
				{
					// Make the packed area rectangle.
					result = std::make_optional<graphics::Rectangle>(space.x, space.y, width, height);

					// Check to see if shape fills completely.
					if (width == space.width && height == space.height)
					{
						// Destroy since not free space anymore.
						std::advance(rit, 1);
						m_free_rects.erase(rit.base());
					}
					else if (width == space.width)
					{
						// If just width fits, shrink new space.
						space.y      += height;
						space.height -= height;
					}
					else if (height == space.height)
					{
						// Same as width, for height.
						space.x     += width;
						space.width -= width;
					}
					else
					{
						// Otherwise, split up existing space.
						graphics::Rectangle temp = {space.x + width, space.y, space.width - width, height};

						space.y      += height;
						space.height -= height;

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

		void RectPack::clear()
		{
			m_free_rects.clear();
			m_free_rects.emplace_back(0, 0, m_width, m_height);
		}

		int RectPack::get_width() const
		{
			return m_width;
		}

		int RectPack::get_height() const
		{
			return m_height;
		}

		const std::vector<graphics::Rectangle>& RectPack::get_free_space() const
		{
			return m_free_rects;
		}
	} // namespace math
} // namespace galaxy
