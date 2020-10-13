///
/// NineSlice.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_NINESLICE_HPP_
#define QUASAR_NINESLICE_HPP_

#include <protostar/graphics/Rect.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Used for the 9 slice image scaling technique.
	///
	struct NineSlice final
	{
		///
		/// Constructor.
		///
		NineSlice() = default;

		///
		/// Copy constructor.
		///
		NineSlice(const NineSlice&);

		///
		/// Move constructor.
		///
		NineSlice(NineSlice&&);

		///
		/// Copy assignment operator.
		///
		NineSlice& operator=(const NineSlice&);

		///
		/// Move assignment operator.
		///
		NineSlice& operator=(NineSlice&&);

		///
		/// Destructor.
		///
		~NineSlice() = default;

		///
		/// [x][ ][ ]
		/// [ ][ ][ ]
		/// [ ][ ][ ]
		///
		pr::Rect<float> m_top_left;

		///
		/// [ ][ ][x]
		/// [ ][ ][ ]
		/// [ ][ ][ ]
		///
		pr::Rect<float> m_top_right;

		///
		/// [ ][ ][ ]
		/// [ ][ ][ ]
		/// [x][ ][ ]
		///
		pr::Rect<float> m_bottom_left;

		///
		/// [ ][ ][ ]
		/// [ ][ ][ ]
		/// [ ][ ][x]
		///
		pr::Rect<float> m_bottom_right;

		///
		/// [ ][x][ ]
		/// [ ][ ][ ]
		/// [ ][ ][ ]
		///
		pr::Rect<float> m_top;

		///
		/// [ ][ ][ ]
		/// [x][ ][ ]
		/// [ ][ ][ ]
		///
		pr::Rect<float> m_left;

		///
		/// [ ][ ][ ]
		/// [ ][ ][x]
		/// [ ][ ][ ]
		///
		pr::Rect<float> m_right;

		///
		/// [ ][ ][ ]
		/// [ ][ ][ ]
		/// [ ][x][ ]
		///
		pr::Rect<float> m_bottom;

		///
		/// [ ][ ][ ]
		/// [ ][x][ ]
		/// [ ][ ][ ]
		///
		pr::Rect<float> m_middle;
	};
} // namespace qs

#endif