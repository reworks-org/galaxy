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
		Sprite();

		///
		/// Copy constructor.
		///
		Sprite(const Sprite&) = delete;

		///
		/// Move constructor.
		///
		Sprite(Sprite&&) = default;

		///
		/// Copy assignment operator.
		///
		Sprite& operator=(const Sprite&) = delete;

		///
		/// Move assignment operator.
		///
		Sprite& operator=(Sprite&&) = default;

		///
		/// Destructor.
		///
		virtual ~Sprite() = default;

		///
		/// \brief Creates the internal vertex array.
		///
		/// BufferType Fixed or dynamic buffer.
		///
		/// \param tex_x Optional texture x pos.
		/// \param tex_y Optional texture y pos.
		///
		template<is_buffer BufferType>
		void create(const float tex_x = 0.0f, const float tex_y = 0.0f);

		///
		/// Activate sprite context.
		///
		void bind() override;

		///
		/// Deactivate sprite context.
		///
		void unbind() override;
	};

	template<is_buffer BufferType>
	inline void Sprite::create(const float tex_x, const float tex_y)
	{
		auto v1 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f, tex_x, tex_y);
		auto v2 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f, tex_x + m_width, tex_y);
		auto v3 = qs::make_vertex<qs::SpriteVertex>(0.0f + m_width, 0.0f + m_height, tex_x + m_width, tex_y + m_height);
		auto v4 = qs::make_vertex<qs::SpriteVertex>(0.0f, 0.0f + m_height, tex_x, tex_y + m_height);

		std::array<unsigned int, 6> ib_arr   = {0, 1, 3, 1, 2, 3};
		std::vector<qs::SpriteVertex> vb_arr = {v1, v2, v3, v4};

		m_vb.create<qs::SpriteVertex, BufferType>(vb_arr);
		m_ib.create<qs::BufferStatic>(ib_arr);

		m_layout.add<qs::SpriteVertex, qs::VAPosition>(2);
		m_layout.add<qs::SpriteVertex, qs::VATexel>(2);

		m_va.create<qs::SpriteVertex>(m_vb, m_ib, m_layout);

		set_rotation_origin(m_width * 0.5f, m_height * 0.5f);
		m_dirty = true;
	}
} // namespace qs

#endif