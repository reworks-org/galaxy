///
/// Vertex.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEX_HPP_
#define QUASAR_VERTEX_HPP_

#include <protostar/system/Colour.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Represents a single vertex of a primitive.
	///
	struct Vertex final
	{
		///
		/// Default constructor.
		///
		Vertex() noexcept;

		///
		/// Argument constructor.
		///
		/// \param x X pos.
		/// \param y Y pos.
		/// \param col Colour.
		/// \param u Texel x pos.
		/// \param v Texel y pos.
		///
		Vertex(const float x, const float y, protostar::Colour& col, const float u, const float v);

		///
		/// Destructor.
		///
		~Vertex() noexcept = default;

		///
		/// Position of vertex.
		///
		float m_position[2];

		///
		/// Colour of vertex.
		///
		float m_colour[4];

		///
		/// Corresponding texture vertex.
		///
		float m_texels[2];
	};
}

#endif