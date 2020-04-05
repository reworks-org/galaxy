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
		/// Define vertexes of sprite.
		///
		/// \param vertexes Vertexes to add to VertexArray.
		///
		template<std::size_t size>
		void addVertexes(const std::array<float, size>& vertexes) noexcept;

		///
		/// Define indexes of sprite.
		///
		/// \param indexes Indexes to add to IndexBuffer.
		///
		template<std::size_t size>
		void addIndexes(const std::array<unsigned int, size>& indexes) noexcept;

		///
		/// Define layout of the VertexArray.
		///
		/// \param stride Total stride of vertex layout.
		/// \param strides Size of each layout in the vertices.
		///
		void addLayout(const unsigned int stride, const std::initializer_list<int>& strides) noexcept;

		///
		/// \brief Creates the internal vertex array.
		///
		/// Call AFTER you have added verticies, indicies and defined the layout.
		///
		void create() noexcept;

		///
		/// Bind VA, texture, and add transform to shader.
		///
		void activate() noexcept;

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

	template<std::size_t size>
	inline void Sprite2D::addVertexes(const std::array<float, size>& vertexes) noexcept
	{
		m_vertexBuffer.create<float, size>(vertexes);
	}

	template<std::size_t size>
	inline void Sprite2D::addIndexes(const std::array<unsigned int, size>& indexes) noexcept
	{
		m_indexBuffer.create<size>(indexes);
	}
}

#endif