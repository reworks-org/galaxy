///
/// RectPack.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ALGORITHM_RECTPACK_HPP_
#define GALAXY_ALGORITHM_RECTPACK_HPP_

#include <optional>
#include <vector>

#include "galaxy/graphics/Rect.hpp"

namespace galaxy
{
	namespace algorithm
	{
		///
		///	Rectangle 2D bin packing class.
		///
		class RectPack final
		{
		public:
			///
			/// Constructor.
			///
			RectPack();

			///
			/// Destructor.
			///
			~RectPack();

			///
			/// Set starting width and height of rectangle.
			///
			/// Generally should be a power of 2.
			///
			/// \param width Width of the master rectangle.
			/// \param height Height of the master rectangle.
			///
			void init(const int width, const int height);

			///
			/// Pack a rectangle into the master rectangle.
			///
			/// \param width Width of the rectangle to pack.
			/// \param height Height of the rectangle to pack.
			///
			/// \return Returns the location of the packed rectangle on the master rectangle.
			///			Otherwise, returns a std::nullopt.
			///
			[[nodiscard]] std::optional<graphics::iRect> pack(const int width, const int height);

			///
			/// Clear all data.
			///
			void clear();

			///
			/// Get total width.
			///
			/// \return Integer.
			///
			[[nodiscard]] int get_width() const;

			///
			/// Get total height.
			///
			/// \return Integer.
			///
			[[nodiscard]] int get_height() const;

			///
			/// Get free rectangles.
			///
			/// \return Const std::vector.
			///
			[[nodiscard]] const std::vector<graphics::iRect>& get_free_space() const;

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
			std::vector<graphics::iRect> m_free_rects;
		};
	} // namespace algorithm
} // namespace galaxy

#endif