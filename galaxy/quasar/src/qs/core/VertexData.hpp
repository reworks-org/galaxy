///
/// VertexData.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXDATA_HPP_
#define QUASAR_VERTEXDATA_HPP_

#include "qs/vertex/VertexArray.hpp"

///
/// Core namespace.
///
namespace qs
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
		/// Copy constructor.
		///
		VertexData(const VertexData&) noexcept = delete;

		///
		/// Move constructor.
		///
		VertexData(VertexData&&);

		///
		/// Copy assignment operator.
		///
		VertexData& operator=(const VertexData&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		VertexData& operator=(VertexData&&);

		///
		/// Virtual destructor.
		///
		virtual ~VertexData() noexcept = default;

		///
		/// Set z-level of sprite.
		///
		/// \param z_level z-ordering level to render sprite at.
		///
		void set_z_level(const unsigned int z_level) noexcept;

		///
		/// Set opacity.
		///
		/// Only affects the currently active quad.
		///
		/// \param opacity Opacity range is from 0.0f (transparent) to 1.0f (opaque).
		///
		void set_opacity(const float opacity) noexcept;

		///
		/// Get z-level of vertexs.
		///
		/// \return Const unsigned integer.
		///
		[[nodiscard]] const unsigned int z_level() const noexcept;

		///
		/// Get opacity.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float opacity() const noexcept;

		///
		/// Get VBO.
		///
		/// \return Reference to VBO.
		///
		[[nodiscard]] qs::VertexBuffer& get_vbo() noexcept;

		///
		/// Get VAO.
		///
		/// \return Reference to VAO.
		///
		[[nodiscard]] qs::VertexArray& get_vao() noexcept;

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
		/// Z-Level
		///
		unsigned int m_z_level;

		///
		/// OpenGL Vertex Array Object.
		///
		qs::VertexArray m_va;

		///
		/// Vertex buffer.
		///
		qs::VertexBuffer m_vb;

		///
		/// Index (Element) buffer.
		///
		qs::IndexBuffer m_ib;

		///
		/// Vertex layout.
		///
		qs::VertexLayout m_layout;
	};
} // namespace qs

#endif