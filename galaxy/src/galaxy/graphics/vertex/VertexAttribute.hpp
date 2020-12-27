///
/// VertexAttribute.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_VERTEX_VERTEXATTRIBUTE_HPP_
#define GALAXY_GRAPHICS_VERTEX_VERTEXATTRIBUTE_HPP_

#include <cstddef>

#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Specifys the attributes of a vertex array for its verticies.
		///
		struct VertexAttribute final
		{
			///
			/// Default constructor.
			///
			VertexAttribute();

			///
			/// \brief Creation constructor.
			///
			/// See: http://docs.gl/gl4/glVertexAttribPointer
			///
			/// \param size Number of components for each vertex attribute.
			/// \param type Data type of the verticies. I.e. GL_FLOAT or GL_INT.
			/// \param normalized Is this vertex normalized? I.e. GL_FALSE or GL_TRUE.
			/// \param offset The offset of this attribute in the verticies.
			///
			VertexAttribute(const unsigned int size, const unsigned int type, unsigned char normalized, const unsigned int offset);

			///
			/// Destructor.
			///
			~VertexAttribute() = default;

			///
			/// Components for each vertex attribute.
			///
			unsigned int m_size;

			///
			/// Data type. I.e. GL_FLOAT or GL_INT.
			///
			unsigned int m_type;

			///
			/// Is this vertex normalized? I.e. GL_FALSE or GL_TRUE.
			///
			unsigned char m_normalized;

			///
			/// The offset of this attribute in the verticies.
			///
			std::size_t m_offset;
		};
	} // namespace graphics
} // namespace galaxy

#endif