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

#include <SFML/Graphics/Rect.hpp>

namespace galaxy
{
	namespace math
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
			[[nodiscard]]
			std::optional<sf::IntRect> pack(const int width, const int height) noexcept;

			///
			/// Clear all data.
			///
			void clear() noexcept;

			///
			/// Get total width.
			///
			/// \return Integer.
			///
			[[nodiscard]]
			int get_width() const noexcept;

			///
			/// Get total height.
			///
			/// \return Integer.
			///
			[[nodiscard]]
			int get_height() const noexcept;

			///
			/// Get free rectangles.
			///
			/// \return Const meta::vector.
			///
			[[nodiscard]]
			const std::vector<sf::IntRect>& get_free_space() const noexcept;

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
			std::vector<sf::IntRect> m_free_rects;
		};
	} // namespace math
} // namespace galaxy

#endif
