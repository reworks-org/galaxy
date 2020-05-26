///
/// SpriteBatch.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SPRITEBATCH_HPP_
#define QUASAR_SPRITEBATCH_HPP_

#include "qs/texture/BaseTexture.hpp"
#include "qs/sprite/BatchedSprite.hpp"

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
		/// \param maxQuads Maximum number of quads allowed for this spritebatch.
		///
		SpriteBatch(const unsigned int maxQuads) noexcept;

		///
		/// Destructor.
		///
		virtual ~SpriteBatch() noexcept;

		///
		/// Set the texture to use.
		///
		/// \param texture Texture to use when drawing batched sprites.
		///
		void setTexture(qs::BaseTexture* texture) noexcept;

		///
		/// Add a batched sprite to the spritebatch.
		///
		/// \param sprite Sprite to add.
		///
		void add(qs::BatchedSprite* sprite) noexcept;

		///
		/// Update spritebatch vertexs.
		///
		void update() noexcept;

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
		/// Get index count.
		///
		/// \return Const unsigned integer.
		///
		const unsigned int getCount() const noexcept override;

		///
		/// Get width.
		///
		/// \return Const uint.
		///
		const unsigned int getWidth() const noexcept;

		///
		/// Get height.
		///
		/// \return Const uint.
		///
		const unsigned int getHeight() const noexcept;

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

	private:
		///
		/// Offset tracker.
		///
		unsigned int m_offset;
		
		///
		/// Max # of quads.
		///
		unsigned int m_maxQuads;

		///
		/// Max # of vertexs.
		///
		unsigned int m_maxVertexs;

		///
		/// Max # of indexs.
		///
		unsigned int m_maxIndexs;

		///
		/// Index count.
		///
		unsigned int m_indexCount;

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
		VertexStorage<qs::SpriteVertex> m_vertexs;
	};
}

#endif