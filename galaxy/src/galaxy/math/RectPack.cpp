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

			m_free_rects.emplace_back(sf::IntRect({0, 0}, {m_width, m_height}));
		}

		std::optional<sf::IntRect> RectPack::pack(const int width, const int height) noexcept
		{
			// Result.
			std::optional<sf::IntRect> result = std::nullopt;

			// Go over each space in the rectangle, in reverse order (i.e. smallest -> largest).
			for (auto rit = m_free_rects.rbegin(); rit != m_free_rects.rend();)
			{
				auto& space = *rit;

				// Check if rect can fit into space.
				if (width <= space.size.x && height <= space.size.y)
				{
					// Make the packed area rectangle.
					result = std::make_optional<sf::IntRect>(sf::IntRect({space.position.x, space.position.y}, {width, height}));

					// Check to see if shape fills completely.
					if (width == space.size.x && height == space.size.y)
					{
						// Destroy since not free space anymore.
						std::advance(rit, 1);
						m_free_rects.erase(rit.base());
					}
					else if (width == space.size.x)
					{
						// If just width fits, shrink new space.
						space.position.y += height;
						space.size.y     -= height;
					}
					else if (height == space.size.y)
					{
						// Same as width, for height.
						space.position.x += width;
						space.size.x     -= width;
					}
					else
					{
						// Otherwise, split up existing space.
						m_free_rects.emplace_back(sf::IntRect({space.position.x + width, space.position.y}, {space.size.x - width, height}));

						space.position.y += height;
						space.size.y     -= height;
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
			m_free_rects.emplace_back(sf::IntRect({0, 0}, {m_width, m_height}));
		}

		int RectPack::get_width() const noexcept
		{
			return m_width;
		}

		int RectPack::get_height() const noexcept
		{
			return m_height;
		}

		const std::vector<sf::IntRect>& RectPack::get_free_space() const noexcept
		{
			return m_free_rects;
		}
	} // namespace math
} // namespace galaxy
