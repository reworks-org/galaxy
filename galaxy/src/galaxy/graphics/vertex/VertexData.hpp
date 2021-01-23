///
/// VertexData.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_VERTEXDATA_HPP_
#define GALAXY_GRAPHICS_VERTEX_VERTEXDATA_HPP_

#include "galaxy/graphics/vertex/VertexArray.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Provides a class with a method of rendering to screen.
		///
		class VertexData
		{
		public:
			///
			/// Constructor.
			///
			VertexData() noexcept;

			///
			/// Move constructor.
			///
			VertexData(VertexData&&) noexcept;

			///
			/// Move assignment operator.
			///
			VertexData& operator=(VertexData&&) noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~VertexData() noexcept = default;

			///
			/// Set opacity.
			///
			/// Only affects the currently active quad.
			///
			/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
			///
			void set_opacity(const float opacity) noexcept;

			///
			/// Get opacity.
			///
			/// \return Const float.
			///
			[[nodiscard]] const float opacity() const noexcept;

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

		protected:
			///
			/// Opacity of sprite.
			///
			float m_opacity;

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

		private:
			///
			/// Copy constructor.
			///
			VertexData(const VertexData&) = delete;

			///
			/// Copy assignment operator.
			///
			VertexData& operator=(const VertexData&) = delete;
		};
	} // namespace graphics
} // namespace galaxy

#endif