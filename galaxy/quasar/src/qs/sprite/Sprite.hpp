///
/// Sprite.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SPRITE_HPP_
#define QUASAR_SPRITE_HPP_

#include "qs/core/Transform.hpp"
#include "qs/core/VertexData.hpp"
#include "qs/texture/Texture.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Everything you need to draw a sprite.
	///
	class Sprite : public qs::VertexData, public qs::Texture, public qs::Transform
	{
	public:
		///
		/// Constructor.
		///
		Sprite() noexcept;

		///
		/// Destructor.
		///
		~Sprite() noexcept = default;

		///
		/// \brief Creates the internal vertex array.
		///
		/// BufferType Fixed or dynamic buffer.
		///
		template<typename BufferType>
		void create() noexcept;

		///
		/// Set opacity.
		///
		/// Only affects the currently active quad.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void setOpacity(float opacity) noexcept;

		///
		/// Get opacity.
		///
		/// \return Const float.
		///
		const float getOpacity() const noexcept;

		///
		/// Activate sprite context.
		///
		void bind() noexcept override;

		///
		/// Deactivate sprite context.
		///
		void unbind() noexcept override;

	protected:
		///
		/// Opacity of sprite.
		///
		float m_opacity;
	};

	template<typename BufferType>
	inline void Sprite::create() noexcept
	{
		auto v1 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f, 0.0f, 0.0f, m_opacity);
		auto v2 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f, 0.0f + m_width, 0.0f, m_opacity);
		auto v3 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f + m_height, 0.0f + m_width, 0.0f + m_height, m_opacity);
		auto v4 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f + m_height, 0.0f, 0.0f + m_height, m_opacity);

		m_vertexBuffer.create<qs::SpriteVertex, BufferType>({v1, v2, v3, v4});
		m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

		m_layout.add<qs::SpriteVertex, qs::VATypePosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATypeTexel>(2);
		m_layout.add<qs::SpriteVertex, qs::VATypeOpacity>(1);

		m_vertexArray.create<qs::SpriteVertex>(m_vertexBuffer, m_indexBuffer, m_layout);

		setRotationOrigin(m_width * 0.5f, m_height * 0.5f);
		setOpacity(1.0f);
	}
}

#endif