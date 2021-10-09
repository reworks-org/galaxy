///
/// Layout.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_LAYOUT_HPP_
#define GALAXY_GRAPHICS_VERTEX_LAYOUT_HPP_

#include <vector>

#include <glad/glad.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/graphics/vertex/Attribute.hpp"
#include "galaxy/meta/GLMeta.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Container for vertex attributes of verticies in order to specify layout of vertexs in their array.
		///
		class VertexLayout final
		{
		public:
			///
			/// Default constructor.
			///
			VertexLayout() noexcept = default;

			///
			/// Destructor.
			///
			~VertexLayout() noexcept;

			///
			/// \brief Adds a vertex attribute to the layout.
			///
			/// VertexType Is the type of vertex having an vertex attribute added.
			/// VertexAttributeType is the type of vertex member the attribute is for.
			///
			/// \param size Number of components for each vertex attribute.
			///
			template<meta::is_vertex VertexType, VertexAttributes va>
			void add(const unsigned int size) noexcept;

			///
			/// Retrieve all attributes.
			///
			/// \return const reference to std::vector<VertexAttribute>.
			///
			[[nodiscard]] const std::vector<VertexAttribute>& get_attributes() const noexcept;

		private:
			///
			/// Container for all the attributes of this layout.
			///
			std::vector<VertexAttribute> m_attributes;
		};

		template<meta::is_vertex VertexType, VertexAttributes va>
		inline void VertexLayout::add(const unsigned int size) noexcept
		{
			// Now to use constexpr to check on compile time the buffer type.
			// This is faster since we dont need to bother checking at runtime.
			// constexpr will discard the branch that is false and it wont be compiled.
			constexpr bool is_prim_vertex   = std::is_same<VertexType, PrimitiveVertex>::value;
			constexpr bool is_batch_vertex  = std::is_same<VertexType, BatchVertex>::value;
			constexpr bool is_sprite_vertex = std::is_same<VertexType, SpriteVertex>::value;
			constexpr bool is_vertex_3d     = std::is_same<VertexType, Vertex3D>::value;

			if constexpr (is_prim_vertex)
			{
				if constexpr (va == VertexAttributes::POSITION)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(PrimitiveVertex, m_pos)));
				}
				else if constexpr (va == VertexAttributes::COLOUR)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(PrimitiveVertex, m_colour)));
				}
				else
				{
					static_assert(false, "Invalid vertex attribute specificed. Primitive Vertex requires position or colour.");
				}
			}
			else if constexpr (is_batch_vertex)
			{
				if constexpr (va == VertexAttributes::POSITION)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(BatchVertex, m_pos)));
				}
				else if constexpr (va == VertexAttributes::TEXEL)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(BatchVertex, m_texels)));
				}
				else if constexpr (va == VertexAttributes::OPACITY)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(BatchVertex, m_opacity)));
				}
				else
				{
					static_assert(false, "Invalid vertex attribute specificed. Batch Vertex requires position, texel, or opacity.");
				}
			}
			else if constexpr (is_sprite_vertex)
			{
				if constexpr (va == VertexAttributes::POSITION)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(SpriteVertex, m_pos)));
				}
				else if constexpr (va == VertexAttributes::TEXEL)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(SpriteVertex, m_texels)));
				}
				else
				{
					static_assert(false, "Invalid vertex attribute specificed. Sprite Vertex requires position, or texel.");
				}
			}
			else if constexpr (is_vertex_3d)
			{
				if constexpr (va == VertexAttributes::POSITION)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(Vertex3D, m_pos)));
				}
				else if constexpr (va == VertexAttributes::NORMAL)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(Vertex3D, m_normal)));
				}
				else if constexpr (va == VertexAttributes::TANGENTS)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(Vertex3D, m_tangents)));
				}
				else if constexpr (va == VertexAttributes::TEXEL)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(Vertex3D, m_texels)));
				}
				else
				{
					static_assert(false, "Invalid vertex attribute specificed. Vertex3D requires position or texel.");
				}
			}
		}
	} // namespace graphics
} // namespace galaxy

#endif