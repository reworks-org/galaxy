///
/// InstancedSprite.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_INSTANCEDSPRITE_HPP_
#define QUASAR_INSTANCEDSPRITE_HPP_

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
	/// Primitive InstancedSprite object.
	/// Everything you need to draw an instanced texture in OpenGL.
	///
	class InstancedSprite final : public qs::Texture, public qs::Transform
	{
	public:
		///
		/// Constructor.
		///
		InstancedSprite() noexcept;

		///
		/// Destructor.
		///
		~InstancedSprite() noexcept override;

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
		/// \brief Set opacity of InstancedSprite.
		///
		/// Only affects the currently active quad.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void setOpacity(float opacity) noexcept;

		///
		/// Bind VA and texture.
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
	};

	template<typename BufferType>
	inline void InstancedSprite::create() noexcept
	{
		auto quad = qs::Vertex::make_quad({ 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height) }, { 0.0f, 0.0f, 0.0f, 1.0f }, 0.0f, 0.0f);

		m_vertexBuffer.create<BufferType>({ quad[0], quad[1], quad[2], quad[3] });
		m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

		m_layout.add<qs::VATypePosition>(2);
		m_layout.add<qs::VATypeColour>(4);
		m_layout.add<qs::VATypeTexel>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
		
		setRotationOrigin(static_cast<float>(m_width), static_cast<float>(m_height));
	}

	template<typename BufferType>
	inline void InstancedSprite::create(const protostar::Rect<float>& texSrc) noexcept
	{
		auto quad = qs::Vertex::make_quad({ 0.0f, 0.0f, texSrc.m_width, texSrc.m_height }, { 0.0f, 0.0f, 0.0f, 1.0f }, texSrc.m_x, texSrc.m_y);

		m_vertexBuffer.create<BufferType>({ quad[0], quad[1], quad[2], quad[3] });
		m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

		m_layout.add<qs::VATypePosition>(2);
		m_layout.add<qs::VATypeColour>(4);
		m_layout.add<qs::VATypeTexel>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);

		setRotationOrigin(texSrc.m_width, texSrc.m_height);
	}
}

#endif