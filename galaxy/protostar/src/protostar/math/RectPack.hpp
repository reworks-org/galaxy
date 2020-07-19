///
/// RectPack.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_RECTPACK_HPP_
#define PROTOSTAR_RECTPACK_HPP_

#include <optional>
#include <ranges>
#include <vector>

#include "protostar/graphics/Rect.hpp"
#include "protostar/system/Concepts.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	///	Rectangle 2D bin packing class.
	///
	template<IsArithmetic Type>
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
		std::optional<pr::Rect<Type>> pack(const int width, const int height);

		///
		/// Get total width.
		///
		const int getWidth() const noexcept;

		///
		/// Get total height.
		///
		const int getHeight() const noexcept;

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
		std::vector<pr::Rect<Type>> m_freeRects;
	};

	template<IsArithmetic Type>
	RectPack<Type>::RectPack() noexcept
	    : m_width(0), m_height(0)
	{
	}

	template<IsArithmetic Type>
	RectPack<Type>::~RectPack() noexcept
	{
		m_freeRects.clear();
	}

	template<IsArithmetic Type>
	inline void RectPack<Type>::init(const int width, const int height) noexcept
	{
		m_width	 = width;
		m_height = height;

		m_freeRects.emplace_back(0, 0, m_width, m_height);
	}

	template<IsArithmetic Type>
	inline std::optional<pr::Rect<Type>> RectPack<Type>::pack(const int width, const int height)
	{
		std::ranges::reverse_view rv {m_freeRects};
		std::optional<pr::Rect<Type>> result = std::nullopt;

		for (auto rect : rv | std::view::filter([](const pr::Rect<Type>& space) {
					 return width <= space.m_width && height <= space.m_height;
				 }))
		{
			// Make the packed area rectangle.
			result = std::make_optional<pr::Rect<Type>>(space.m_x, space.m_y, width, height);

			// Check to see if shape fills completely.
			if (width == space.m_width && height == space.m_height)
			{
				// Destroy since not free space anymore.
				std::advance(rit, 1);
				m_freeRects.erase(rit.base());
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
				m_freeRects.emplace_back(space.m_x + width, space.m_y, space.m_width - width, height);
				space.m_y += height;
				space.m_height -= height;
			}

			// If it can fit, no longer need to keep iterating through.
			break;
		}

		return std::move(result);
	}

	template<IsArithmetic Type>
	inline const int RectPack<Type>::getWidth() const noexcept
	{
		return m_width;
	}

	template<IsArithmetic Type>
	inline const int RectPack<Type>::getHeight() const noexcept
	{
		return m_height;
	}
} // namespace pr

#endif