///
/// Sprite.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SPRITE_HPP_
#define QUASAR_SPRITE_HPP_

#include "qs/core/Texture.hpp"
#include "qs/core/Transform.hpp"
#include "qs/vertex/VertexArray.hpp"
#include "qs/vertex/VertexLayout.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Primitive sprite object.
	/// Everything you need to draw a texture in OpenGL.
	///
	class Sprite final : public qs::Transform, public qs::Texture
	{
	public:
		///
		/// Constructor.
		///
		Sprite() noexcept;

		///
		/// Destructor.
		///
		~Sprite() noexcept override;

		///
		/// \brief Creates the internal vertex array.
		///
		/// Call AFTER you have loaded a texture.
		///
		/// \param bufferType Fixed or dynamic buffer.
		///
		void create(const qs::BufferType bufferType) noexcept;

		///
		/// \brief Creates the internal vertex array. With custom texture size.
		///
		/// Call AFTER you have loaded a texture.
		///
		/// \param bufferType Fixed or dynamic buffer.
		/// \param texSrc Texture rectangle to use - usually from a spritesheet.
		/// \param quadCount Number of quads being created. This is only for DYNAMIC_DRAW.
		///
		void create(const qs::BufferType bufferType, const protostar::Rect<float>& texSrc) noexcept;

		///
		/// \brief Creates the internal vertex array using an array of vertexs.
		///
		/// Call AFTER you have loaded a texture.
		///
		/// \param vertexs Array of vertexs to create vbo and vao from. Assumes that vertexs represent quads.
		///				I.e. 4 vertex = 1 quad.
		///
		void create(const VertexQuadStorage& vertexs) noexcept;

		///
		/// Bind VA and texture.
		///
		void bind() noexcept override;

		///
		/// Unbinds VA and texture.
		///
		void unbind() noexcept override;

		///
		/// Recalculates the model view matrix.
		///
		void recalculate() noexcept override;

		///
		/// Get vertex array object.
		///
		/// \return Returns reference to internal qs::VertexArray.
		///
		qs::VertexArray& getVAO() noexcept;

		///
		/// Get index count.
		///
		/// \return Const unsigned integer.
		///
		const unsigned int getCount() const noexcept;

	private:
		///
		/// \brief Apply transformation to vertexbuffer.
		///
		/// Only needed for DYNAMIC vertexs. Should only be done when needing to update transformation.
		///
		void applyTransform() noexcept;

	private:
		///
		/// OpenGL Vertex Array Object.
		///
		qs::VertexArray m_vertexArray;

		///
		/// Vertex buffer.
		///
		qs::VertexBuffer m_vertexBuffer;

		///
		/// Index (Element) buffer.
		///
		qs::IndexBuffer m_indexBuffer;

		///
		/// Vertex layout.
		///
		qs::VertexLayout m_layout;

		///
		/// Storage of std::vector to prevent recurrent initialization + creation.
		///
		std::vector<qs::Vertex> m_transformedVertexs;
	};
}

#endif