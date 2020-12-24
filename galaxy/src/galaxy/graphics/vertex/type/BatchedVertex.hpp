///
/// BatchedVertex.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_BATCHEDVERTEX_HPP_
#define QUASAR_BATCHEDVERTEX_HPP_

#include <glm/vec2.hpp>
#include <galaxy/system/Concepts.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Represents a single BatchedVertex of a primitive.
	///
	struct BatchedVertex final
	{
		///
		/// Default constructor.
		///
		BatchedVertex();

		///
		/// Argument constructor.
		///
		/// \param x X pos.
		/// \param y Y pos.
		/// \param u Texel x pos.
		/// \param v Texel y pos.
		/// \param opacity 1.0f = Opaque, 0.0f = Transparent.
		///
		BatchedVertex(const float x, const float y, const float u, const float v, const float opacity);

		///
		/// Copy constructor.
		///
		BatchedVertex(const BatchedVertex&);

		///
		/// Move constructor.
		///
		BatchedVertex(BatchedVertex&&);

		///
		/// Copy assignment operator.
		///
		BatchedVertex& operator=(const BatchedVertex&);

		///
		/// Move assignment operator.
		///
		BatchedVertex& operator=(BatchedVertex&&);

		///
		/// Destructor.
		///
		~BatchedVertex() = default;

		///
		/// Position of BatchedVertex.
		///
		glm::vec2 m_pos;

		///
		/// BatchedVertex texels.
		///
		glm::vec2 m_texels;

		///
		/// Opacity.
		///
		float m_opacity;
	};
} // namespace qs

#endif