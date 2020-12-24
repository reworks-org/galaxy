///
/// PrimitiveVertex.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_PRIMITIVEVERTEX_HPP_
#define QUASAR_PRIMITIVEVERTEX_HPP_

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <galaxy/graphics/Colour.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Represents a single PrimitiveVertex of a primitive.
	///
	class PrimitiveVertex final
	{
	public:
		///
		/// Default constructor.
		///
		PrimitiveVertex();

		///
		/// Argument constructor.
		///
		/// \param x X pos.
		/// \param y Y pos.
		/// \param col Colour.
		///
		PrimitiveVertex(const float x, const float y, pr::Colour& col);

		///
		/// Copy constructor.
		///
		PrimitiveVertex(const PrimitiveVertex&);

		///
		/// Move constructor.
		///
		PrimitiveVertex(PrimitiveVertex&&);

		///
		/// Copy assignment operator.
		///
		PrimitiveVertex& operator=(const PrimitiveVertex&);

		///
		/// Move assignment operator.
		///
		PrimitiveVertex& operator=(PrimitiveVertex&&);

		///
		/// Destructor.
		///
		~PrimitiveVertex() = default;

		///
		/// Position of PrimitiveVertex.
		///
		glm::vec2 m_pos;

		///
		/// Colour of PrimitiveVertex.
		///
		glm::vec4 m_colour;
	};
} // namespace qs

#endif