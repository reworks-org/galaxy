///
/// Renderable.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERABLE_HPP_
#define QUASAR_RENDERABLE_HPP_

#include "qs/core/Texture.hpp"
#include "qs/vertex/VertexArray.hpp"
#include "qs/vertex/VertexLayout.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Provides a class with a method of rendering to screen.
	///
	class Renderable : public qs::Texture
	{
	public:
		///
		/// Constructor.
		///
		Renderable() noexcept = default;

		///
		/// Virtual destructor.
		///
		virtual ~Renderable() noexcept = default;

		///
		/// \brief Creates the internal vertex array.
		///
		/// BufferType Fixed or dynamic buffer.
		///
		template<typename BufferType>
		void create() noexcept;

		///
		/// \brief Creates the internal vertex array. Specify position on creation.
		///
		/// BufferType Fixed or dynamic buffer.
		///
		/// \param x Custom x pos.
		/// \param y Custom y pos.
		///
		template<typename BufferType>
		void create(const float x, const float y) noexcept;

		///
		/// \brief Creates the internal vertex array. With custom texture src.
		///
		/// BufferType Fixed or dynamic buffer.
		///
		/// \param src Texture rectangle to use - usually from a spritesheet.
		///
		template<typename BufferType>
		void create(const protostar::Rect<float>& src) noexcept;

		///
		/// \brief Creates the internal vertex array. Specify position and texture src on creation.
		///
		/// BufferType Fixed or dynamic buffer.
		///
		/// \param x Custom x pos.
		/// \param y Custom y pos.
		/// \param src Texture rectangle to use - usually from a spritesheet.
		///
		template<typename BufferType>
		void create(const float x, const float y, const protostar::Rect<float>& src) noexcept;

		///
		/// Binds VA and texture.
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

	protected:
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
	inline void Renderable::create() noexcept
	{
		auto quad = qs::Vertex::make_quad({ 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height) }, { 0.0f, 0.0f, 0.0f, 1.0f }, 0.0f, 0.0f);

		m_vertexBuffer.create<BufferType>({ quad[0], quad[1], quad[2], quad[3] });
		m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

		m_layout.add<qs::VATypePosition>(2);
		m_layout.add<qs::VATypeColour>(4);
		m_layout.add<qs::VATypeTexel>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	template<typename BufferType>
	inline void Renderable::create(const float x, const float y) noexcept
	{
		auto quad = qs::Vertex::make_quad({ x, y, static_cast<float>(m_width), static_cast<float>(m_height) }, { 0.0f, 0.0f, 0.0f, 1.0f }, 0.0f, 0.0f);

		m_vertexBuffer.create<BufferType>({ quad[0], quad[1], quad[2], quad[3] });
		m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

		m_layout.add<qs::VATypePosition>(2);
		m_layout.add<qs::VATypeColour>(4);
		m_layout.add<qs::VATypeTexel>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	template<typename BufferType>
	inline void Renderable::create(const protostar::Rect<float>& src) noexcept
	{
		auto quad = qs::Vertex::make_quad({ 0.0f, 0.0f, src.m_width, src.m_height }, { 0.0f, 0.0f, 0.0f, 1.0f }, src.m_x, src.m_y);

		m_vertexBuffer.create<BufferType>({ quad[0], quad[1], quad[2], quad[3] });
		m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

		m_layout.add<qs::VATypePosition>(2);
		m_layout.add<qs::VATypeColour>(4);
		m_layout.add<qs::VATypeTexel>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	template<typename BufferType>
	inline void Renderable::create(const float x, const float y, const protostar::Rect<float>& src) noexcept
	{
		auto quad = qs::Vertex::make_quad({ x, y, src.m_width, src.m_height }, { 0.0f, 0.0f, 0.0f, 1.0f }, src.m_x, src.m_y);

		m_vertexBuffer.create<BufferType>({ quad[0], quad[1], quad[2], quad[3] });
		m_indexBuffer.create<qs::BufferTypeStatic>({ 0, 1, 3, 1, 2, 3 });

		m_layout.add<qs::VATypePosition>(2);
		m_layout.add<qs::VATypeColour>(4);
		m_layout.add<qs::VATypeTexel>(2);

		m_vertexArray.create(m_vertexBuffer, m_indexBuffer, m_layout);
	}
}

#endif