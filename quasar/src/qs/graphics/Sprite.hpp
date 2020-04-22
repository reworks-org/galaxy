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
	class Sprite final : public qs::Texture
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
		/// \brief When applying a change to a quad in the sprite, you can choose a specific quad.
		///
		/// Defaults to first quad (i.e. offset 0).
		/// Only need when you have more than 1 quad.
		///
		/// \param offset Offset of the quad in the vertexarray. Remember each quad is 4 vertexs.
		///			      So quad 1 would be 0, and quad 2 would be 4.
		///
		void setActiveQuad(const int offset = 0);

		///
		/// \brief Translate (move) position.
		///
		/// Does not set the position but moves it.
		/// By adding the parameters to the existing transformation matrix.
		///
		/// \param x How far to translate on x axis.
		/// \param y How far to translate on x axis.
		///
		void move(const float x, const float y);

		///
		/// Rotate transformation in degrees.
		///
		/// \param degrees Max 360, min -360.
		///
		void rotate(const float degrees) noexcept;

		///
		/// Scale transformation on each axis.
		///
		/// \param scale Scale multiplier.
		///
		void scale(const float scale) noexcept;

		///
		/// \brief Set opacity of sprite.
		///
		/// Only affects the currently active quad.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void setOpacity(float opacity) noexcept;

		///
		/// Apply all transforms to modified quads.
		///
		void applyTransforms();

		///
		/// Bind VA and texture.
		/// And also updates the transforms.
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
		/// Get index count.
		///
		/// \return Const unsigned integer.
		///
		const unsigned int getCount() const noexcept;

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
		/// Current active offset.
		///
		int m_activeOffset;

		///
		/// Transforms for each quad in vertex array.
		///
		std::unordered_map<int, qs::Transform> m_transforms;

		///
		/// Currently active transform.
		///
		qs::Transform* m_activeTransform;

		///
		/// Copy of vertexbuffer vertexs for transforming.
		///
		VertexStorage m_transformVertexs;
	};
}

#endif