///
/// Sprite.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SPRITE_HPP_
#define QUASAR_SPRITE_HPP_

#include "qs/texture/Texture.hpp"
#include "qs/core/Transform.hpp"
#include "qs/core/VertexData.hpp"

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
		Sprite();

		///
		/// Destructor.
		///
		virtual ~Sprite() noexcept = default;

		///
		/// \brief Creates the internal vertex array.
		///
		/// buffer_type Fixed or dynamic buffer.
		///
		template<typename buffer_type>
		void create();

		///
		/// Set opacity.
		///
		/// Only affects the currently active quad.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void set_opacity(pr::between_1_and_0 auto opacity) noexcept;

		///
		/// Get opacity.
		///
		/// \return Const float.
		///
		const float opacity() const noexcept;

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

	template<typename buffer_type>
	inline void Sprite::create()
	{
		auto v1 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f, 0.0f, 0.0f, m_opacity);
		auto v2 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f, 0.0f + m_width, 0.0f, m_opacity);
		auto v3 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f + m_height, 0.0f + m_width, 0.0f + m_height, m_opacity);
		auto v4 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f + m_height, 0.0f, 0.0f + m_height, m_opacity);

		m_vb.create<qs::SpriteVertex, buffer_type>({v1, v2, v3, v4});
		m_ib.create<qs::BufferTypeStatic>({0, 1, 3, 1, 2, 3});

		m_layout.add<qs::SpriteVertex, qs::VATypePosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATypeTexel>(2);
		m_layout.add<qs::SpriteVertex, qs::VATypeOpacity>(1);

		m_va.create<qs::SpriteVertex>(m_vb, m_ib, m_layout);

		set_rotation_origin(m_width * 0.5f, m_height * 0.5f);
		m_opacity = 1.0f;
		m_dirty   = true;
	}
} // namespace qs

#endif