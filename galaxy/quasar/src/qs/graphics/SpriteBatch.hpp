///
/// SpriteBatch.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SPRITEBATCH_HPP_
#define QUASAR_SPRITEBATCH_HPP_

#include "qs/core/Transform.hpp"
#include "qs/renderer/Renderable.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Everything you need to draw a SpriteBatch.
	///
	class SpriteBatch final : public qs::Renderable
	{
		///
		/// So update() does not need to be exposed publicly.
		///
		friend class Renderer;

	public:
		///
		/// Constructor.
		///
		SpriteBatch() noexcept;

		///
		/// Destructor.
		///
		~SpriteBatch() noexcept override;

		///
		/// \brief Creates the internal vertex array using an array of quads.
		///
		/// Quad = Array of vertexs of size 4.
		///
		/// \param vertexs Array of vertexs to create vbo and vao from.
		///
		void create(const qs::VertexQuadStorage& vertexs) noexcept;

		///
		/// Get a transform for a quad.
		///
		/// \param offset Offset of the quad. Quad 1 is 0, quad 2 is 1, etc.
		///				  Is linear based on how quads are added. I.e. in same order.
		///
		qs::Transform* getTransform(const int offset) noexcept;

	private:
		///
		/// Updates spritebatch with changes.
		///
		void update() noexcept;

	private:	
		///
		/// Offset of the quad. Quad 1 is 0, quad 2 is 1, etc.
		///	Is linear based on how quads are added. I.e. in same order.
		///
		std::vector<qs::Transform> m_transforms;

		///
		/// Stores transformed vertexs.
		///
		qs::VertexStorage m_transfVB;
	};
}

#endif