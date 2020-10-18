///
/// NineSlice.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_NINESLICE_HPP_
#define QUASAR_NINESLICE_HPP_

#include <glm/vec2.hpp>

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
		/// The width/height of each cell. They should be a 3x3 grid of the same size.
		///
		glm::vec2 m_cell_size;
	};
} // namespace qs

#endif