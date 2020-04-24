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
		/// \brief Creates the internal vertex array. With custom texture size.
		///
		/// Call AFTER you have loaded a texture.
		/// BufferType Fixed or dynamic buffer.
		///
		template<typename BufferType>
		void create() noexcept;

		///
		/// \brief Creates the internal vertex array. With custom texture size.
		///
		/// Call AFTER you have loaded a texture.
		/// BufferType Fixed or dynamic buffer.
		///
		/// \param texSrc Texture rectangle to use - usually from a spritesheet.
		///
		template<typename BufferType>
		void create(const protostar::Rect<float>& texSrc) noexcept;

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

	template<typename BufferType>
	inline void Sprite::create() noexcept
	{
		auto quad = qs::Vertex::make_quad({ 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height) }, { 0.0f, 0.0f, 0.0f, 1.0f }, 0.0f, 0.0f);

		m_vertexBuffer.create<BufferType>({ quad[0], quad[1], quad[2], quad[3] });
		m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

		m_layout.add<qs::VATypePosition>(2);
		m_layout.add<qs::VATypeColour>(4);
		m_layout.add<qs::VATypeTexel>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);

		m_transforms.emplace(0, std::move(qs::Transform()));
		m_transforms[0].setRotationOrigin(static_cast<float>(m_width), static_cast<float>(m_height));

		setActiveQuad();
	}

	template<typename BufferType>
	inline void Sprite::create(const protostar::Rect<float>& texSrc) noexcept
	{
		auto quad = qs::Vertex::make_quad({ 0.0f, 0.0f, texSrc.m_width, texSrc.m_height }, { 0.0f, 0.0f, 0.0f, 1.0f }, texSrc.m_x, texSrc.m_y);

		m_vertexBuffer.create<BufferType>({ quad[0], quad[1], quad[2], quad[3] });
		m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

		m_layout.add<qs::VATypePosition>(2);
		m_layout.add<qs::VATypeColour>(4);
		m_layout.add<qs::VATypeTexel>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);

		m_transforms.emplace(0, std::move(qs::Transform()));
		m_transforms[0].setRotationOrigin(texSrc.m_width, texSrc.m_height);

		setActiveQuad();
	}
}

#endif