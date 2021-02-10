///
/// SpriteBatch.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SPRITEBATCH_HPP_
#define GALAXY_GRAPHICS_SPRITEBATCH_HPP_

#include "galaxy/components/BatchedSprite.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/graphics/texture/BaseTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		///  Draw a batch of vertex data with a texture in one draw call.
		///
		class SpriteBatch final
		{
		public:
			///
			/// Argument constructor.
			///
			/// \param max_quads Maximum number of quads allowed for this spritebatch.
			///
			SpriteBatch(const unsigned int max_quads);

			///
			/// Move constructor.
			///
			SpriteBatch(SpriteBatch&&) noexcept;

			///
			/// Move assignment operator.
			///
			SpriteBatch& operator=(SpriteBatch&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~SpriteBatch() noexcept;

			///
			/// Set the texture to use.
			///
			/// \param texture Texture to use when drawing batched sprites. Must not be nullptr.
			///
			void set_texture(BaseTexture* texture) noexcept;

			///
			/// Add a batched sprite to the spritebatch.
			///
			/// \param sprite Sprite to add. Must not be nullptr.
			/// \param transform Transform for the batched sprite.
			/// \param z_level Z level of batched sprite for sorting later.
			///
			void add(components::BatchedSprite* sprite, components::Transform* transform, const int z_level);

			///
			/// Calculate spritebatch vertexs.
			///
			void calculate();

			///
			/// Calculate spritebatch vertexs.
			///
			/// \param global_transform Takes in a transform to apply to all batches.
			///
			void calculate(components::Transform* global_transform);

			///
			/// Frees up sprites without clearing texture or other important data.
			///
			void clear_sprites() noexcept;

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
			/// Get IBO.
			///
			/// \return Reference to IBO.
			///
			[[nodiscard]] IndexBuffer& get_ibo() noexcept;

			///
			/// Get VBO.
			///
			/// \return Reference to VBO.
			///
			[[nodiscard]] VertexBuffer& get_vbo() noexcept;

			///
			/// Get VAO.
			///
			/// \return Reference to VAO.
			///
			[[nodiscard]] VertexArray& get_vao() noexcept;

			///
			/// Get index count.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int index_count() const noexcept;

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

			///
			/// Is the spritebatch empty.
			///
			/// \return Const bool. True if empty.
			///
			[[nodiscard]] const bool empty() const noexcept;

		private:
			///
			/// Constructor.
			///
			SpriteBatch() = delete;

			///
			/// Copy constructor.
			///
			SpriteBatch(const SpriteBatch&) = delete;

			///
			/// Copy assignment operator.
			///
			SpriteBatch& operator=(const SpriteBatch&) = delete;

			///
			/// \brief Sort sprites by z level.
			///
			/// Sprites are sorted according to z level.
			/// Those with same level are left in the order that they are in when they are added with begin().
			///
			void sort() noexcept;

		private:
			///
			/// OpenGL Vertex Array Object.
			///
			VertexArray m_va;

			///
			/// Vertex buffer.
			///
			VertexBuffer m_vb;

			///
			/// Index (Element) buffer.
			///
			IndexBuffer m_ib;

			///
			/// Vertex layout.
			///
			VertexLayout m_layout;

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
			BaseTexture* m_texture;

			///
			/// Array of batched sprites.
			///
			std::vector<std::pair<components::BatchedSprite*, components::Transform*>> m_sprites;

			///
			/// Vertexs of spritebatch.
			///
			std::vector<BatchedVertex> m_vertexs;
		};
	} // namespace graphics
} // namespace galaxy

#endif