///
/// SpriteBatch.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SPRITEBATCH_HPP_
#define QUASAR_SPRITEBATCH_HPP_

#include "qs/sprite/BatchedSprite.hpp"
#include "qs/texture/BaseTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Everything you need to draw a SpriteBatch.
	///
	class SpriteBatch final : public qs::VertexData
	{
	public:
		///
		/// Argument constructor.
		///
		/// \param max_quads Maximum number of quads allowed for this spritebatch.
		///
		SpriteBatch(const unsigned int max_quads);

		///
		/// Copy constructor.
		///
		SpriteBatch(const SpriteBatch&) noexcept = delete;

		///
		/// Move constructor.
		///
		SpriteBatch(SpriteBatch&&) noexcept = delete;

		///
		/// Copy assignment operator.
		///
		SpriteBatch& operator=(const SpriteBatch&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		SpriteBatch& operator=(SpriteBatch&&) noexcept = delete;

		///
		/// Destructor.
		///
		virtual ~SpriteBatch() noexcept;

		///
		/// Set the texture to use.
		///
		/// \param texture Texture to use when drawing batched sprites. Must not be nullptr.
		///
		void set_texture(qs::BaseTexture* texture) noexcept;

		///
		/// Add a batched sprite to the spritebatch.
		///
		/// \param sprite Sprite to add. Must not be nullptr.
		///
		void add(qs::BatchedSprite* sprite);

		///
		/// Update spritebatch vertexs.
		///
		void update();

		///
		/// Clears the spritebatch of data.
		///
		void clear() noexcept;

		///
		/// Bind to OpenGL.
		///
		void bind() noexcept;

		///
		/// Unbind from OpenGL.
		///
		void unbind() noexcept;

		///
		/// Gets the number of actual used indicies in a spritebatch.
		///
		/// \return Const uint.
		///
		[[nodiscard]] const unsigned int get_used_index_count() const noexcept;

		///
		/// Get width.
		///
		/// \return Const uint.
		///
		[[nodiscard]] const unsigned int get_width() const noexcept;

		///
		/// Get height.
		///
		/// \return Const uint.
		///
		[[nodiscard]] const unsigned int get_height() const noexcept;

	private:
		///
		/// Constructor.
		///
		SpriteBatch() noexcept = delete;

		///
		/// \brief Sort sprites by z level.
		///
		/// Sprites are sorted according to z level.
		/// Those with same level are left in the order that they are in when they are added with begin().
		///
		void sort() noexcept;

		///
		/// Offset tracker.
		///
		unsigned int m_offset;

		///
		/// Max # of quads.
		///
		unsigned int m_max_quads;

		///
		/// Max # of vertexs.
		///
		unsigned int m_max_vertexs;

		///
		/// Max # of indexs.
		///
		unsigned int m_max_indexs;

		///
		/// Index count.
		///
		unsigned int m_used_indexs;

		///
		/// Pointer to texture to use.
		///
		qs::BaseTexture* m_texture;

		///
		/// Array of batched sprites.
		///
		std::vector<qs::BatchedSprite*> m_sprites;

		///
		/// Vertexs of spritebatch.
		///
		std::vector<qs::SpriteVertex> m_vertexs;
	};
} // namespace qs

#endif