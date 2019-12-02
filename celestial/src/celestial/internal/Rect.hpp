///
/// Rect.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef RECT_CELESTIAL_HPP_
#define RECT_CELESTIAL_HPP_

namespace celestial
{
	namespace internal
	{
		///
		/// Basic rectangle interface.
		///
		template<typename T>
		struct Rect final
		{
			///
			/// Default constructor.
			///
			Rect() = default;

			///
			/// Constructor.
			///
			Rect(const T x, const T y, const T w, const T h);

			///
			/// Destructor.
			///
			~Rect() noexcept = default;

			///
			/// x value.
			///
			T m_x = 0;

			///
			/// y value.
			///
			T m_y = 0;

			///
			/// width value.
			///
			T m_w = 0;

			///
			/// height value.
			///
			T m_h = 0;
		};

		template<typename T>
		inline Rect<T>::Rect(const T x, const T y, const T w, const T h)
			:m_x(x), m_y(y), m_w(w), m_h(h)
		{
		}
	}

	///
	/// Shortcut syntax.
	///
	template<typename T>
	using Rect = celestial::internal::Rect<T>;
}

#endif