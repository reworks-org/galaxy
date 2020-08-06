///
/// VertexAttribute.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_VERTEXATTRIBUTE_HPP_
#define QUASAR_VERTEXATTRIBUTE_HPP_

#include <cstddef>

#include <protostar/system/Concepts.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Specifys the attributes of a vertex array for its verticies.
	///
	struct VertexAttribute final
	{
		///
		/// Default constructor.
		///
		VertexAttribute() noexcept;

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
		VertexAttribute(const pr::positive_integer auto size, const pr::positive_integer auto type, unsigned char normalized, const pr::positive_integer auto offset) noexcept;

		///
		/// Components for each vertex attribute.
		///
		int m_size;

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
} // namespace qs

#endif