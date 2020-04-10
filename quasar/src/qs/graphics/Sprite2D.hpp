///
/// Sprite2D.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SPRITE2D_HPP_
#define QUASAR_SPRITE2D_HPP_

#include "qs/core/Texture.hpp"
#include "qs/vertex/VertexArray.hpp"
#include "qs/transforms/Transform.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Primitive sprite object.
	/// Everything you need to draw a texture in OpenGL.
	///
	class Sprite2D final : public qs::Transform, public qs::Texture
	{
	public:
		///
		/// Constructor.
		///
		Sprite2D() noexcept;

		///
		/// Destructor.
		///
		~Sprite2D() noexcept override;

		///
		/// \brief Creates the internal vertex array.
		///
		/// Call AFTER you have loaded a texture.
		///
		void create() noexcept;

		///
		/// Bind VA and texture.
		///
		void bind() noexcept override;

		///
		/// Unbinds VA and texture.
		///
		void unbind() noexcept override;

		///
		/// Get vertex array object.
		///
		/// \return Returns reference to internal qs::VertexArray.
		///
		qs::VertexArray& getVAO() noexcept;

		///
		/// Get index buffer.
		///
		/// \return Reference to qs::IndexBuffer.
		///
		qs::IndexBuffer& getIBO() noexcept;

	private:
		///
		/// OpenGL Vertex Buffer Object.
		///
		qs::VertexBuffer m_vertexBuffer;

		///
		/// OpenGL index (element) buffer.
		///
		qs::IndexBuffer m_indexBuffer;

		///
		/// Defines layout of verticies.
		///
		qs::VertexLayout m_layout;

		///
		/// OpenGL Vertex Array Object.
		///
		qs::VertexArray m_vertexArray;
	};
}

#endif