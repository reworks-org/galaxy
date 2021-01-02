///
/// VertexLayout.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_VERTEXLAYOUT_HPP_
#define GALAXY_GRAPHICS_VERTEX_VERTEXLAYOUT_HPP_

#include <vector>

#include <glad/glad.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/graphics/GraphicsConcepts.hpp"
#include "galaxy/graphics/vertex/VertexAttribute.hpp"

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
			VertexLayout();

			///
			/// Destructor.
			///
			~VertexLayout();

			///
			/// \brief Adds a vertex attribute to the layout.
			///
			/// VertexType Is the type of vertex having an vertex attribute added.
			/// VertexAttributeType is the type of vertex member the attribute is for.
			///
			/// \param size Number of components for each vertex attribute.
			///
			template<is_vertex VertexType, is_vertex_attribute VertexAttribute>
			void add(const unsigned int size);

			///
			/// Retrieve all attributes.
			///
			/// \return const reference to std::vector<VertexAttribute>.
			///
			[[nodiscard]] const std::vector<VertexAttribute>& get_attributes() const;

		private:
			///
			/// Container for all the attributes of this layout.
			///
			std::vector<VertexAttribute> m_attributes;
		};

		template<is_vertex VertexType, is_vertex_attribute VertexAttribute>
		inline void VertexLayout::add(const unsigned int size)
		{
			// Now to use constexpr to check on compile time the buffer type.
			// This is faster since we dont need to bother checking at runtime.
			// constexpr will discard the branch that is false and it wont be compiled.
			constexpr bool is_prim_vertex   = std::is_same<VertexType, PrimitiveVertex>::value;
			constexpr bool is_sprite_vertex = std::is_same<VertexType, SpriteVertex>::value;
			constexpr bool is_batch_vertex  = std::is_same<VertexType, BatchedVertex>::value;

			if constexpr (is_sprite_vertex)
			{
				if constexpr (std::is_same<VertexAttribute, VAPosition>::value)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(SpriteVertex, m_pos)));
				}
				else if constexpr (std::is_same<VertexAttribute, VATexel>::value)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(SpriteVertex, m_texels)));
				}
				else
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to add vertex attribute for sprite vertex.");
				}
			}
			else if constexpr (is_prim_vertex)
			{
				if constexpr (std::is_same<VertexAttribute, VAPosition>::value)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(PrimitiveVertex, m_pos)));
				}
				else if constexpr (std::is_same<VertexAttribute, VAColour>::value)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(PrimitiveVertex, m_colour)));
				}
				else
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to add vertex attribute for primitive vertex.");
				}
			}
			else if constexpr (is_batch_vertex)
			{
				if constexpr (std::is_same<VertexAttribute, VAPosition>::value)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(BatchedVertex, m_pos)));
				}
				else if constexpr (std::is_same<VertexAttribute, VATexel>::value)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(BatchedVertex, m_texels)));
				}
				else if constexpr (std::is_same<VertexAttribute, VAOpacity>::value)
				{
					m_attributes.emplace_back(size, static_cast<unsigned int>(GL_FLOAT), static_cast<unsigned char>(GL_FALSE), static_cast<unsigned int>(offsetof(BatchedVertex, m_opacity)));
				}
				else
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to add vertex attribute for batched vertex.");
				}
			}
		}
	} // namespace graphics
} // namespace galaxy

#endif