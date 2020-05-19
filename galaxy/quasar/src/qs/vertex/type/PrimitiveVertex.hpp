///
/// PrimitiveVertex.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_PRIMITIVEVERTEX_HPP_
#define QUASAR_PRIMITIVEVERTEX_HPP_

#include <glm/vec2.hpp>
#include <protostar/system/Colour.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Represents a single PrimitiveVertex of a primitive.
	///
	struct PrimitiveVertex final
	{
		///
		/// Default constructor.
		///
		PrimitiveVertex() noexcept;

		///
		/// Argument constructor.
		///
		/// \param x X pos.
		/// \param y Y pos.
		/// \param col Colour.
		///
		explicit PrimitiveVertex(const float x, const float y, protostar::Colour& col) noexcept;

		///
		/// Destructor.
		///
		~PrimitiveVertex() noexcept = default;

		///
		/// Position of PrimitiveVertex.
		///
		glm::vec2 m_pos;

		///
		/// Colour of PrimitiveVertex.
		///
		protostar::Colour m_colour;
	};
}

#endif