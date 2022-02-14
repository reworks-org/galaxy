///
/// RectPack.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "RectPack.hpp"

namespace galaxy
{
	namespace algorithm
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

		std::optional<glm::vec4> RectPack::pack(const int width, const int height) noexcept
		{
			// Result.
			std::optional<glm::vec4> result = std::nullopt;

			// Go over each space in the rectangle, in reverse order (i.e. smallest -> largest).
			for (auto rit = m_free_rects.rbegin(); rit != m_free_rects.rend(); /* ++rit*/)
			{
				auto& space = *rit;

				// Check if rect can fit into space.
				if (width <= space.z && height <= space.w)
				{
					// Make the packed area rectangle.
					result = std::make_optional<glm::vec4>(space.x, space.y, width, height);

					// Check to see if shape fills completely.
					if (width == space.z && height == space.w)
					{
						// Destroy since not free space anymore.
						std::advance(rit, 1);
						m_free_rects.erase(rit.base());
					}
					else if (width == space.z)
					{
						// If just width fits, shrink new space.
						space.y += height;
						space.w -= height;
					}
					else if (height == space.w)
					{
						// Same as width, for height.
						space.x += width;
						space.z -= width;
					}
					else
					{
						// Otherwise, split up existing space.
						glm::vec4 temp = {space.x + width, space.y, space.z - width, height};

						space.y += height;
						space.w -= height;

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

		const int RectPack::get_width() const noexcept
		{
			return m_width;
		}

		const int RectPack::get_height() const noexcept
		{
			return m_height;
		}

		const std::vector<glm::vec4>& RectPack::get_free_space() const noexcept
		{
			return m_free_rects;
		}
	} // namespace algorithm
} // namespace galaxy