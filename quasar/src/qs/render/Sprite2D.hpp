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
	class Sprite2D final : public qs::Transform
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
		/// \brief Add the vertices to the sprite.
		///
		/// MUST BE FLOATS.
		///
		template<typename ...Vertices>
		void addVertices() noexcept;

		///
		/// \brief Add the indicies to the sprite.
		///
		/// MUST BE FLOATS.
		///
		template<typename ...Indicies>
		void addIndices() noexcept;

		///
		/// Define layout of the VertexArray.
		///
		/// \param strides Size of each layout in the vertices.
		///
		void addLayout(const std::initializer_list<int>& strides) noexcept;

		///
		/// \brief Creates the internal vertex array.
		///
		/// Call AFTER you have added verticies, indicies and defined the layout.
		///
		void create() noexcept;

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

		///
		/// Texture.
		///
		qs::Texture m_texture;
	};

	template<typename ...Vertices>
	inline void Sprite2D::addVertices() noexcept
	{
		// Some compile time voodoo in this function.
		static_assert(std::is_same<float, std::decay_t<Vertices>>::value);

		// Check at compile time that the vertices is not empty.
		constexpr std::size_t size = sizeof...(Vertices);
		if constexpr (size > 0)
		{
			// Create at compile time. Cool.
			constexpr std::array<float, size> arr = { Vertices... };
			m_vertexBuffer.create(arr);
		}
	}

	template<typename ...Indicies>
	inline void Sprite2D::addIndices() noexcept
	{
		// Some compile time voodoo in this function.
		static_assert(std::is_same<float, std::decay_t<Indicies>>::value);

		// Check at compile time that the vertices is not empty.
		constexpr std::size_t size = sizeof...(Indicies);
		if constexpr (size > 0)
		{
			// Create at compile time. Cool.
			constexpr std::array<float, size> arr = { Indicies... };
			m_indexBuffer.create(arr);
		}
	}
}

#endif