///
/// RectPack.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_RECTPACK_HPP_
#define GALAXY_MATH_RECTPACK_HPP_

#include <optional>
#include <vector>

#include "galaxy/math/Rect.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace math
	{
		///
		///	Rectangle 2D bin packing class.
		///
		template<meta::is_arithmetic Type>
		class RectPack final
		{
		public:
			///
			/// Constructor.
			///
			RectPack() noexcept;

			///
			/// Destructor.
			///
			~RectPack() noexcept;

			///
			/// Set starting width and height of rectangle.
			///
			/// Generally should be a power of 2.
			///
			/// \param width Width of the master rectangle.
			/// \param height Height of the master rectangle.
			///
			void init(const int width, const int height) noexcept;

			///
			/// Pack a rectangle into the master rectangle.
			///
			/// \param width Width of the rectangle to pack.
			/// \param height Height of the rectangle to pack.
			///
			/// \return Returns the location of the packed rectangle on the master rectangle.
			///			Otherwise, returns a std::nullopt.
			///
			[[nodiscard]] std::optional<math::Rect<Type>> pack(const int width, const int height);

			///
			/// Clear all data.
			///
			void clear();

			///
			/// Get total width.
			///
			[[nodiscard]] const int get_width() const noexcept;

			///
			/// Get total height.
			///
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// Get free rectangles.
			///
			/// \return Const std::vector.
			///
			[[nodiscard]] const std::vector<math::Rect<Type>>& get_free_space() const noexcept;

		private:
			///
			/// The starting width of the rectangle.
			///
			int m_width;

			///
			/// The starting width of the rectangle.
			///
			int m_height;

			///
			/// Free space in master rectangle.
			///
			std::vector<math::Rect<Type>> m_free_rects;
		};

		template<meta::is_arithmetic Type>
		inline RectPack<Type>::RectPack() noexcept
			: m_width {0}
			, m_height {0}
		{
		}

		template<meta::is_arithmetic Type>
		inline RectPack<Type>::~RectPack() noexcept
		{
			m_free_rects.clear();
		}

		template<meta::is_arithmetic Type>
		inline void RectPack<Type>::init(const int width, const int height) noexcept
		{
			m_width  = width;
			m_height = height;

			m_free_rects.emplace_back(0, 0, m_width, m_height);
		}

		template<meta::is_arithmetic Type>
		inline std::optional<math::Rect<Type>> RectPack<Type>::pack(const int width, const int height)
		{
			// Result.
			std::optional<math::Rect<Type>> result = std::nullopt;

			// Go over each space in the rectangle, in reverse order (i.e. smallest -> largest).
			for (auto rit = m_free_rects.rbegin(); rit != m_free_rects.rend(); /* ++rit*/)
			{
				auto& space = *rit;

				// Check if rect can fit into space.
				if (width <= space.m_width && height <= space.m_height)
				{
					// Make the packed area rectangle.
					result = std::make_optional<math::Rect<Type>>(space.m_x, space.m_y, width, height);

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
						space.m_y += height;
						space.m_height -= height;
					}
					else if (height == space.m_height)
					{
						// Same as width, for height.
						space.m_x += width;
						space.m_width -= width;
					}
					else
					{
						// Otherwise, split up existing space.
						math::Rect<Type> temp = {space.m_x + width, space.m_y, space.m_width - width, height};

						space.m_y += height;
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

		template<meta::is_arithmetic Type>
		inline void RectPack<Type>::clear()
		{
			m_free_rects.clear();
			m_free_rects.emplace_back(0, 0, m_width, m_height);
		}

		template<meta::is_arithmetic Type>
		inline const int RectPack<Type>::get_width() const noexcept
		{
			return m_width;
		}

		template<meta::is_arithmetic Type>
		inline const int RectPack<Type>::get_height() const noexcept
		{
			return m_height;
		}

		template<meta::is_arithmetic Type>
		inline const std::vector<math::Rect<Type>>& RectPack<Type>::get_free_space() const noexcept
		{
			return m_free_rects;
		}
	} // namespace math
} // namespace galaxy

#endif