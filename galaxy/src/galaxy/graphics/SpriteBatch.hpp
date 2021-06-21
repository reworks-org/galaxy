///
/// SpriteBatch.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SPRITEBATCH_HPP_
#define GALAXY_GRAPHICS_SPRITEBATCH_HPP_

#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/components/Transform2D.hpp"
#include "galaxy/graphics/VertexArray.hpp"

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
			/// Constructor.
			///
			SpriteBatch() noexcept;

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
			~SpriteBatch() noexcept;

			///
			/// Add a batch sprite and its transform to this spritebatch.
			///
			/// \param sprite Pointer to batched sprite component.
			/// \param transform Pointer to transform component.
			///
			void add(components::BatchSprite* sprite, components::Transform2D* transform);

			///
			/// Set the texture for this spritebatch.
			///
			/// \param texture OpenGL texture ID.
			///
			void add_texture(const unsigned int texture);

			///
			/// Copy data from CPU to GPU.
			///
			void buffer_data();

			///
			/// Bind to OpenGL.
			///
			void bind() noexcept;

			///
			/// Unbind from OpenGL.
			///
			void unbind() noexcept;

			///
			/// Clears the spritebatch of data.
			///
			void clear() noexcept;

			[[nodiscard]] const int get_width() const noexcept;
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// Get index count.
			///
			/// \return Const unsigned integer.
			///
			[[nodiscard]] const unsigned int count() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			SpriteBatch(const SpriteBatch&) = delete;

			///
			/// Copy assignment operator.
			///
			SpriteBatch& operator=(const SpriteBatch&) = delete;

		private:
			///
			/// Width cache.
			///
			int m_width;

			///
			/// Height cache.
			///
			int m_height;

			///
			/// GL texture this spritebatch uses.
			///
			unsigned int m_texture;

			///
			/// Vertex data.
			///
			VertexArray m_vao;

			///
			/// Array of vertices.
			///
			std::vector<Vertex> m_vertices;
		};
	} // namespace graphics
} // namespace galaxy

#endif